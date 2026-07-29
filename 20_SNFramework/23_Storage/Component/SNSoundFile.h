#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNPCM.h"

// 画像ファイルクラス
class SNSoundFile : public SNFile, public SNPCM
{
public:
	// コンストラクタ
	SNSoundFile();

	// デストラクタ
	virtual ~SNSoundFile();

	// デコード(非同期実行)
	// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
	virtual Void Decode();

	// 処理完了？
	virtual Boolean IsOperationComplete();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();
};
