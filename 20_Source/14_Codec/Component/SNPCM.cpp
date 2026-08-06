#include "SNPCM.h"
#include "SNSoundCodec.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"

// PCMクラス

// コンストラクタ
SNPCM::SNPCM()
{
    Channels = SNSystemConfig::PCMChannel;
    SampleRate = SNSystemConfig::PCMSampleRate;
    BitPerSample = SNSystemConfig::PCMBitPerSample;

    TargetData = nullptr;

	return;
}

// デストラクタ
SNPCM::~SNPCM()
{
    DeletePCM();

	return;
}

// DIB生成
Void SNPCM::CreatePCM(UInt32 size)
{
    DeletePCM();

    Channels = SNSystemConfig::PCMChannel;
    SampleRate = SNSystemConfig::PCMSampleRate;
    BitPerSample = SNSystemConfig::PCMBitPerSample;

    PCMData.Allocate(size);
    PCMData.Clear();

    return;
}


// DIB解放
Void SNPCM::DeletePCM()
{
    PCMData.Free();

    return;
}

// WICBitmap取得
UInt8* SNPCM::GetPCM()
{
    return (UInt8*)PCMData.GetAddress();
}

// サイズ取得
UInt32 SNPCM::GetSize()
{
    return PCMData.GetSize();
}


// デコード(非同期実行)
// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
Void SNPCM::Decode(SNMemory* in_data)
{
	TargetData = in_data;
	RequestOperation(SNSoundCodecOperationDecode);
	return;
}

SNSoundCodecResult SNPCM::OnOperationSoundDecode()
{
	SNSoundCodec::Decode(TargetData, this);

    return SNSoundCodecResultNormal;
}
