#include "SNSoundCodec.h"
#include "SNWindowsAPI.h"
#include "SNSoundCodecThread.h"
#include "SNAutoResource.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

SNThread* SNSoundCodec::CodecThread;
volatile Boolean SNSoundCodec::ThreadEndRequest = false;		// スレッド終了要求
SNList SNSoundCodec::RequestList;				// リクエストリスト
SNCriticalSection SNSoundCodec::ListAccessCS;	// リストアクセス用クリティカルセクション



Void SNSoundCodec::Initialize()
{
	ListAccessCS.Initialize();
	RequestList.Allocate(SNSystemConfig::CodecRequestMax);

    CreateThread();

    return;
}

Void SNSoundCodec::CreateThread()
{
    ReleaseThread();
    CodecThread = new SNSoundCodecThread();
	CodecThread->Run();
    return;
}


Void SNSoundCodec::Terminate()
{
    ReleaseThread();
    return;
}

Void SNSoundCodec::ReleaseThread()
{
    if (CodecThread != nullptr)
    {
		// 終了要求セット
		ThreadEndRequest = true;
		
		// 終了待ち(MAX1秒)
		CodecThread->WaitForThreadEnd();

        delete CodecThread;
        CodecThread = nullptr;
    }

    return;
}

// ストレージスレッドメイン
Void SNSoundCodec::UserMain()
{
	SNSoundCodecAccessor* request;

	// ストレージスレッドメインループ
	while (true)
	{
		// 終了要求がセットされたら抜ける
		if (ThreadEndRequest)
		{
			break;
		}

		// リクエスト取得
		request = GetRequest();
		if (request != nullptr)
		{
			request->Callback();
		}

		// 処理後、いったんCPU解放
		::Sleep(1);
	}

	return;
}

// リクエスト
Void SNSoundCodec::Request(SNSoundCodecAccessor* request)
{
	SNListContainer* list;

	{
		SNAutoResource cs(&ListAccessCS);

		// リストの最後にリクエストを追加
		list = RequestList.InsertLast();
		if (list != nullptr)
		{
			list->UserData = (Void*)request;
		}
	}

	return;
}

// リクエスト取得
SNSoundCodecAccessor* SNSoundCodec::GetRequest()
{
	SNListContainer* list;
	SNSoundCodecAccessor* ret = nullptr;

	{
		SNAutoResource cs(&ListAccessCS);

		// リスト先頭からリクエストを取得しリスト削除
		list = RequestList.GetTop();

		if (list != nullptr)
		{
			ret = (SNSoundCodecAccessor*)list->UserData;
			RequestList.RemoveTop();
		}
	}

	return ret;
}

Void SNSoundCodec::Decode(SNMemory* in, SNPCM* out)
{
    SNList memoryblocklist;
    IMFSourceReader* reader;

    // デコード結果を格納するメモリブロック生成
    memoryblocklist.Allocate(SNSystemConfig::DecodeBlockNum);

    // Readerを生成
    reader = (IMFSourceReader*)CreateReaderFromMemory(in);

    // デコード実行
    UInt32 totalsize = DecodeToBlocks((Handle)reader, &memoryblocklist);

    // メモリブロックからSNPCMを生成
    BuildPCMFromBlocks(memoryblocklist, totalsize, out);

    // メタデータ設定
    SetPCMMeta(reader, out);

    // Reader解放
    reader->Release();
    return;
}

Boolean SNSoundCodec::ReadSampleOneShot(Handle reader, Handle* sample)
{
    IMFSourceReader* rd = (IMFSourceReader*)reader;
    Boolean ret = false;

    IMFSample* smp = nullptr;
    UInt32 streamIndex = 0;
    UInt32 flags = 0;
    Int64 timestamp = 0;
    HRESULT hr;

    // デコード実行
    hr = rd->ReadSample(
        MF_SOURCE_READER_FIRST_AUDIO_STREAM,
        0,
        &streamIndex,
        &flags,
        &timestamp,
        &smp);

    if (FAILED(hr))
    {
        ret = true;
    }
    else if ((flags & MF_SOURCE_READERF_ENDOFSTREAM))
    {
        ret = true;
    }
    else
    {
        *sample = (Handle)smp;
    }

    return ret;
}

Void SNSoundCodec::InitDecodePos(Handle reader)
{
    IMFSourceReader* rd = (IMFSourceReader*)reader;
    PROPVARIANT var;

    PropVariantInit(&var);
    var.vt = VT_I8;
    var.hVal.QuadPart = 0;
    rd->SetCurrentPosition(GUID_NULL, var);

    return;
}

Void SNSoundCodec::LockSampleBuffer(Handle sample, Handle* buffer, UInt8** buffer_adr, UInt32* size)
{
    IMFSample* smp = (IMFSample*)sample;
    IMFMediaBuffer* mf_buffer = nullptr;
    UInt8* audio_data = nullptr;
    UInt32 audio_len = 0;

    smp->ConvertToContiguousBuffer(&mf_buffer);

    audio_data = nullptr;
    audio_len = 0;

    mf_buffer->Lock(&audio_data, nullptr, &audio_len);

    *buffer = mf_buffer;
    *buffer_adr = audio_data;
    *size = audio_len;

    return;
}

Void SNSoundCodec::CopySample(UInt8* sample, UInt32 src_offset, UInt8* block, UInt32 dst_offset, UInt32 size)
{
    CopyMemory(
        block + dst_offset,
        sample + src_offset,
        size);

    return;
}

Void SNSoundCodec::ReleaseSample(Handle sample, Handle buffer)
{
    IMFSample* smp = (IMFSample*)sample;
    IMFMediaBuffer* mf_buffer = (IMFMediaBuffer*)buffer;

    if (mf_buffer != nullptr)
    {
        mf_buffer->Unlock();
        mf_buffer->Release();
    }

    if (smp != nullptr)
    {
        smp->Release();
    }

    return;
}

Void SNSoundCodec::GetPCMMeta(Handle reader, UInt32* ch, UInt32* bits, UInt32* rate)
{
    IMFSourceReader* rd = (IMFSourceReader*)reader;
    IMFMediaType* actual_type = nullptr;
    UINT32 ui32_ch;
    UINT32 ui32_bits;
    UINT32 ui32_rate;

    // Readerからtypeを取得
    rd->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &actual_type);

    // メタ情報取得
    actual_type->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &ui32_ch);
    actual_type->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &ui32_bits);
    actual_type->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &ui32_rate);

    *ch = ui32_ch;
    *bits = ui32_bits;
    *rate = ui32_rate;    

    actual_type->Release();

    return;
}


// ストリームオープン
Void SNSoundCodec::OpenStream(SNMemory* in, SNPCMStream* out)
{
    IMFSourceReader* reader = nullptr;

    CloseStream(out);

    // MP3をストリーミング用にコピー
    out->Source.Allocate(in->GetSize());
    out->Source.Copy(in->GetAddress(), in->GetSize());

    reader = (IMFSourceReader*)CreateReaderFromMemory(&out->Source);

    // StreamクラスにStreamとReaderを設定
    out->Reader = reader;

    return;
}

// ストリームクローズ(同期呼び出し可)
Void SNSoundCodec::CloseStream(SNPCMStream* stream)
{
    IMFSourceReader* reader = (IMFSourceReader*)stream->Reader;

    if (reader != nullptr)
    {
        reader->Release();
        stream->Reader = nullptr;
    }

    // MP3 の元データを解放
    stream->Source.Free();  

    return;
}



// MP3データからReaderを生成
Handle SNSoundCodec::CreateReaderFromMemory(SNMemory* in)
{
    HGLOBAL hmem = nullptr;
    void* pmem = nullptr;
    IStream* mp3_stream = nullptr;
    IMFByteStream* byte_stream = nullptr;
    IMFSourceReader* reader = nullptr;
    IMFMediaType* pcm_type = nullptr;

    // MP3バッファをHGLOBALにコピー
    hmem = ::GlobalAlloc(GMEM_MOVEABLE, in->GetSize());
    pmem = ::GlobalLock(hmem);
    CopyMemory(pmem, in->GetAddress(), in->GetSize());
    ::GlobalUnlock(hmem);

    // HGLOBALからStream/ Readerを生成
    CreateStreamOnHGlobal(hmem, TRUE, &mp3_stream);
    MFCreateMFByteStreamOnStream(mp3_stream, &byte_stream);
    MFCreateSourceReaderFromByteStream(byte_stream, nullptr, &reader);

    // Readerに出力フォーマット(PCM)を設定
    MFCreateMediaType(&pcm_type);
    pcm_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pcm_type->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pcm_type);

    // Reader以外は不要なので解放
    pcm_type->Release();
    byte_stream->Release();
    mp3_stream->Release();

    return reader;
}

// デコードを行い結果をメモリブロックに設定する
UInt32 SNSoundCodec::DecodeToBlocks(Handle in_reader, SNList* memoryblocklist)
{
    SNListContainer* it = nullptr;
    IMFSample* sample = nullptr;
    UInt32 totalsize = 0;
    IMFMediaBuffer* buffer = nullptr;
    UInt8* audio_data = nullptr;
    UInt32 audio_len = 0;
    UInt32 dst_offset = 0;
    UInt32 src_offset = 0;
    Int64 src_remain = 0;

    UInt32 copy_size = 0;

    it = memoryblocklist->InsertLast();
    it->UserData = (Void*)new UInt8[SNSystemConfig::DecodeBlockSize];

    // デコード処理
    while (!ReadSample(in_reader, (Handle*)&sample))
    {
        sample->ConvertToContiguousBuffer(&buffer);

        audio_data = nullptr;
        audio_len = 0;

        buffer->Lock(&audio_data, nullptr, &audio_len);

        // PCM を DecodeBlockSize ごとに memoryblocklist に積む
        src_remain = audio_len;
        src_offset = 0;

        while (src_remain > 0)
        {
            copy_size = (UInt32)SNMath::SelectMin(src_remain, SNSystemConfig::DecodeBlockSize - dst_offset);

            CopyMemory(
                (UInt8*)it->UserData + dst_offset,
                audio_data + src_offset,
                copy_size
            );

            dst_offset += copy_size;
            src_offset += copy_size;
            src_remain -= copy_size;

            // ブロック満杯なら次ブロックを用意
            if (dst_offset >= SNSystemConfig::DecodeBlockSize)
            {
                it = memoryblocklist->InsertLast();
                it->UserData = (Void*)new UInt8[SNSystemConfig::DecodeBlockSize];
                dst_offset = 0;
            }

            // 途中で抜けた場合を考慮し、ブロックごとに合計サイズを加算する
            totalsize += copy_size;
        }

        buffer->Unlock();
        buffer->Release();
        sample->Release();
    }
    return totalsize;
}

Boolean SNSoundCodec::ReadSample(Handle in_reader, Handle* out_sample)
{
    IMFSourceReader* reader = (IMFSourceReader*)in_reader;
    Boolean end_decode = false;
    UInt32 streamIndex = 0;
    UInt32 flags = 0;
    Int64 timestamp = 0;
    IMFSample* sample = nullptr;
    HRESULT hr;

    // デコード実行してsample取得するか終了するまで繰り返す
    while ((sample == nullptr) && (!end_decode))
    {
        // デコード実行
        hr = reader->ReadSample(
            MF_SOURCE_READER_FIRST_AUDIO_STREAM,
            0,
            &streamIndex,
            &flags,
            &timestamp,
            &sample
        );

        if (FAILED(hr))
        {
            end_decode = true;
        }
        else if ((flags & MF_SOURCE_READERF_ENDOFSTREAM))
        {
            end_decode = true;
        }
    }

    *out_sample = sample;

    return end_decode;
}

Void SNSoundCodec::BuildPCMFromBlocks(SNList& memoryblocklist, UInt32 totalsize, SNPCM* out)
{
    UInt32 offset = 0;
    UInt32 copysize = 0;
    Int64 cnt;
    SNListContainer* it = nullptr;

    out->CreatePCM(totalsize);

    // totalsize分コピーするまでループする
    for (cnt = totalsize; cnt > 0; cnt -= copysize)
    {
        it = memoryblocklist.GetTop();
        if (it == nullptr)
        {
            // 先にループ終了条件にかかるのでココで抜けることはない
            break;
        }

        copysize = (UInt32)SNMath::SelectMin(cnt, SNSystemConfig::DecodeBlockSize);

        CopyMemory(&out->GetPCM()[offset], it->UserData, copysize);
        offset += copysize;

        delete[] it->UserData;
        memoryblocklist.RemoveTop();
    }

    return;
}

Void SNSoundCodec::SetPCMMeta(Handle in_reader, SNPCM* out)
{
    IMFSourceReader* reader = (IMFSourceReader*)in_reader;
    IMFMediaType* actual_type = nullptr;
    UINT32 channels = 0;
    UINT32 bits = 0;
    UINT32 samplerate = 0;

    // Readerからtypeを取得
    reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &actual_type);

    // メタ情報取得
    actual_type->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &channels);
    actual_type->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &bits);
    actual_type->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &samplerate);

    out->Channels = channels;
    out->BitPerSample = bits;
    out->SampleRate = samplerate;

    actual_type->Release();

    return;
}
