#include "SNPCMStream.h"
#include "SNSoundCodec.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

// PCMStreamクラス

// PCMBlockの構造

// PCMBlockStore
// + SNListContainer
//   + SNMemory

// PCMBlockList
// + SNListContainer
//   + SNListContainer(Store)
//     + SNMemory

// コンストラクタ
SNPCMStream::SNPCMStream()
{
    Channels = SNSystemConfig::PCMChannel;
    SampleRate = SNSystemConfig::PCMSampleRate;
    BitPerSample = SNSystemConfig::PCMBitPerSample;
    Reader = nullptr;

    PCMBlockStore.CreateResourceFunc = CreateMemoryBlock;
    PCMBlockStore.DeleteResourceFunc = DeleteMemoryBlock;

    PCMBlockStore.CreateStore(SNSystemConfig::StreamingBlockNum);
    PCMBlockList.Allocate(SNSystemConfig::StreamingBlockNum);

    TargetData = nullptr;

    DecodePhase = SNPCMStreamDecodePhaseSeek;
    DecodeSourceInfo.Sample = nullptr;
    DecodeSourceInfo.Buffer = nullptr;
    DecodeSourceInfo.BufferAdr = nullptr;
    DecodeSourceInfo.Size = 0;
    DecodeSourceInfo.Offset = 0;
    DecodeTargetInfo.Block = nullptr;
    DecodeTargetInfo.Size = 0;
    DecodeTargetInfo.Used = 0;

	return;
}

// デストラクタ
SNPCMStream::~SNPCMStream()
{
    ClosePCMStream();

	return;
}

Void SNPCMStream::OpenPCMStream(SNMemory* in_data)
{
    ClosePCMStream();

    Channels = SNSystemConfig::PCMChannel;
    SampleRate = SNSystemConfig::PCMSampleRate;
    BitPerSample = SNSystemConfig::PCMBitPerSample;

    TargetData = in_data;
    RequestOperation(SNSoundCodecOperationOpenStream);

    return;
}


Void SNPCMStream::ClosePCMStream()
{
    // ストリームをクローズ
    SNSoundCodec::CloseStream(this);

    // メモリブロック解放
    ReleaseAllPCMBlock();

    return;
}

Void SNPCMStream::ReleaseAllPCMBlock()
{
    Int32 num;
    Int32 count;
    SNListContainer* it;

    num = PCMBlockList.GetNum();

    // 使用中のメモリブロックをすべてプールに戻す
    for (count = 0; count < num; count++)
    {
        it = PCMBlockList.GetTop();
        ReleaseStreamBlock((SNListContainer*)it->UserData);
        PCMBlockList.RemoveTop();
    }

    return;
}


// デコード開始
Void SNPCMStream::StartDecode()
{
    ReleaseAllPCMBlock();

    // デコード情報初期化
    DecodePhase = SNPCMStreamDecodePhaseSeek;
    DecodeSourceInfo.Sample = nullptr;
    DecodeSourceInfo.Buffer = nullptr;
    DecodeSourceInfo.BufferAdr = nullptr;
    DecodeSourceInfo.Size = 0;
    DecodeSourceInfo.Offset = 0;
    DecodeTargetInfo.Block = nullptr;
    DecodeTargetInfo.Size = 0;
    DecodeTargetInfo.Used = 0;

    SNSoundCodec::GetPCMMeta(Reader, (UInt32*)&Channels, (UInt32*)&BitPerSample, (UInt32*)&SampleRate);

    return;
}


// デコード処理
Void SNPCMStream::Decode()
{
    Boolean end_flg;
    SNListContainer* list;;
    SNPCMStreamSourceInfo* source = &DecodeSourceInfo;
    SNPCMStreamTargetInfo* target = &DecodeTargetInfo;
    UInt32 copy_size;

    switch (DecodePhase)
    {
    case SNPCMStreamDecodePhaseSeek:
        // 先頭にSEEK
        SNSoundCodec::InitDecodePos(Reader);
        DecodePhase = SNPCMStreamDecodePhaseRead;
        break;

    case SNPCMStreamDecodePhaseRead:

        // 未処理サンプルあり
        if (source->Offset < source->Size)
        {
            DecodePhase = SNPCMStreamDecodePhaseGetBlock;
        }

        // 未処理サンプルなし
        else
        {
            // サンプル読み込み
            end_flg = SNSoundCodec::ReadSampleOneShot(Reader, &source->Sample);

            // Stream終端
            if (end_flg)
            {
                // 先頭に戻す
                DecodePhase = SNPCMStreamDecodePhaseSeek;
            }

            // サンプル有効
            else if (source->Sample != nullptr)
            {
                DecodePhase = SNPCMStreamDecodePhaseSampleLock;
            }
            else
            {
                // 現行フェーズ継続
            }
        }
        break;

    case SNPCMStreamDecodePhaseSampleLock:
        SNSoundCodec::LockSampleBuffer(
            source->Sample,
            &source->Buffer,
            &source->BufferAdr,
            &source->Size);
        source->Offset = 0;

        DecodePhase = SNPCMStreamDecodePhaseGetBlock;
        break;

    case SNPCMStreamDecodePhaseGetBlock:

        // ブロック取得済み
        if (target->Block != nullptr)
        {
            DecodePhase = SNPCMStreamDecodePhaseSetBlock;
        }

        // ブロック未取得
        else
        {
            // ブロック取得
            target->Block = PCMBlockStore.GetResource();
            if (target->Block != nullptr)
            {
                target->Size = ((SNMemory*)target->Block->UserData)->GetSize();
                target->Used = 0;
            }
        }

        break;

    case SNPCMStreamDecodePhaseSetBlock:
        // 未処理サンプルデータあり
        if (source->Offset < source->Size)
        {
            // ブロックに空きあり
            if (target->Used < target->Size)
            {
                copy_size = (UInt32)SNMath::SelectMin(source->Size - source->Offset, target->Size - target->Used);

                // サンプルをブロックにコピー
                SNSoundCodec::CopySample(source->BufferAdr, source->Offset, (UInt8*)((SNMemory*)(target->Block->UserData))->GetAddress(), target->Used, copy_size);

                source->Offset += copy_size;
                target->Used += copy_size;
            }

            // ブロックがいっぱいならリスト登録へ
            else
            {
                DecodePhase = SNPCMStreamDecodePhaseSetList;
            }
        }

        // サンプル処理完了
        else
        {
            // サンプル解放
            SNSoundCodec::ReleaseSample(source->Sample, source->Buffer);

            source->Sample = nullptr;
            source->Buffer = nullptr;
            source->BufferAdr = nullptr;
            source->Offset = 0;
            source->Size = 0;
            DecodePhase = SNPCMStreamDecodePhaseRead;
        }

        break;

    case SNPCMStreamDecodePhaseSetList:
        list = PCMBlockList.InsertLast();
        list->UserData = target->Block;
        target->Block = nullptr;
        target->Size = 0;
        target->Used = 0;
        DecodePhase = SNPCMStreamDecodePhaseRead;
        break;
    }

    return;
}

Void SNPCMStream::DecodeFull()
{
    // ブロックが貯まるまでループする
    while (PCMBlockList.GetNum() < SNSystemConfig::StreamingBlockNum)
    {
        Decode();
    }

    return;
}

// デコード開始
Void SNPCMStream::EndDecode()
{
    // デコード情報初期化
    DecodePhase = SNPCMStreamDecodePhaseSeek;

    SNSoundCodec::ReleaseSample(DecodeSourceInfo.Sample, DecodeSourceInfo.Buffer);

    ReleaseAllPCMBlock();

    return;
}


SNListContainer* SNPCMStream::GetStreamBlock()
{
    SNListContainer* it_list;
    SNListContainer* ret = nullptr;

    // リスト先頭を取得
    it_list = (SNListContainer*)PCMBlockList.GetTop();

    if (it_list != nullptr)
    {
        ret = ((SNListContainer*)it_list->UserData);
        PCMBlockList.RemoveTop();
    }

    return ret;
}

Void SNPCMStream::ReleaseStreamBlock(SNListContainer* block)
{
   // 指定ブロックをプールに戻す
    PCMBlockStore.ReleaseResource(block);

    return;
}

SNSoundCodecResult SNPCMStream::OnOperationOpenStream()
{
	SNSoundCodec::OpenStream(TargetData, this);

    return SNSoundCodecResultNormal;
}

Void* SNPCMStream::CreateMemoryBlock()
{
    SNMemory* mem = new SNMemory();

    mem->Allocate(SNSystemConfig::StreamingBlockSize);

    return mem;
}

Void SNPCMStream::DeleteMemoryBlock(Void* res)
{
    SNMemory* mem = (SNMemory*)res;

    mem->Free();

    delete mem;

    return;
}
