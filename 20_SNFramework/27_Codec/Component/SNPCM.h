#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundCodecAccessor.h"
#include "SNMemory.h"

// PCMクラス

class SNPCM :public SNSoundCodecAccessor
{
public:
	// コンストラクタ
	SNPCM();

	// デストラクタ
	virtual ~SNPCM();

	// PCM生成
	virtual Void CreatePCM(UInt32 size);

	// PCM解放
	virtual Void DeletePCM();

	// PCM取得
	virtual UInt8* GetPCM();

	// サイズ取得
	virtual UInt32 GetSize();

	// デコード(非同期実行)
	// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
	virtual Void Decode(SNMemory* in_data);

	Int32 Channels;
	Int32 SampleRate;
	Int32 BitPerSample;
	SNMemory PCMData;

	SNMemory* TargetData;

protected:
	virtual SNSoundCodecResult OnOperationSoundDecode();

private:
};
