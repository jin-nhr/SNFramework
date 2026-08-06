#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNPCM.h"

// 音声ファイルクラス
class SNSoundFile : public SNFile, public SNPCM
{
public:
	// コンストラクタ
	SNSoundFile();

	// デストラクタ
	virtual ~SNSoundFile();

	// デコード(非同期実行)
	virtual Void Decode();

	// 処理完了？
	virtual Boolean IsOperationComplete();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();
};
