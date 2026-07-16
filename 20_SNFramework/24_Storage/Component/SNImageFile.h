#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNDIB.h"


// 画像ファイルクラス
class SNImageFile : public SNFile, public SNDIB
{
public:
	// コンストラクタ
	SNImageFile();

	// デストラクタ
	virtual ~SNImageFile();

	// デコード(非同期実行)
	// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
	virtual Void Decode();

	// エンコード(非同期実行)
	// エンコード結果を受け取るメモリオブジェクトを指定する
	virtual Void Encode();

	// 処理完了？
	virtual Boolean IsOperationComplete();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();
};
