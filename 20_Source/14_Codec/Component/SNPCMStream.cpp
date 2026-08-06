#include "SNPCMStream.h"
#include "SNSoundCodec.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"
#include "SNAutoResource.h"

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
    Stream = nullptr;
    Reader = nullptr;

    CS.Initialize();
    
    PCMBlockStore.CreateResourceFunc = CreateMemoryBlock;
    PCMBlockStore.DeleteResourceFunc = DeleteMemoryBlock;

    PCMBlockStore.CreateStore(SNSystemConfig::StreamingBlockNum);
    PCMBlockList.Allocate(SNSystemConfig::StreamingBlockNum);

    TargetData = nullptr;

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
    SNAutoResource cs(&CS);

    num = PCMBlockList.GetNum();

    // 使用中のメモリブロックをすべてプールに戻す
    for (count = 0; count < num; count++)
    {
        ReleaseStreamBlock();
    }

    return;
}

// デコード処理
Void SNPCMStream::Decode(Boolean reset)
{
    if (reset)
    {
        // デコード情報の初期化
        PROPVARIANT var;
        PropVariantInit(&var);
        var.vt = VT_I8;
        var.hVal.QuadPart = 0;
        ((IMFSourceReader*)Reader)->SetCurrentPosition(GUID_NULL, var);

        // Working のオフセットも初期化
        Working.Free();
    }

    // デコード処理を実行
    SNSoundCodec::MusicDecode(this);

    return;
}

SNMemory* SNPCMStream::GetStreamBlock()
{
    SNListContainer* it;
    SNMemory* ret = nullptr;

    // リスト先頭を取得
    it = (SNListContainer*)PCMBlockList.GetTop()->UserData;

    if (it != nullptr)
    {
        ret = (SNMemory*)it->UserData;
    }

    return ret;
}

Void SNPCMStream::ReleaseStreamBlock()
{
    SNListContainer* it;

    // リスト先頭を取得
    it = (SNListContainer*)PCMBlockList.GetTop()->UserData;

    if (it != nullptr)
    {
        // 先頭ブロックをリストから削除
        PCMBlockList.RemoveTop();

        // 取得したリストをプールに戻す
        PCMBlockStore.ReleaseResource(it);
    }

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
