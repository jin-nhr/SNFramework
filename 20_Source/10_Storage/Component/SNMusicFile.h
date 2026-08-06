#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNPCMStream.h"

// Musicファイルクラス
class SNMusicFile : public SNFile, public SNPCMStream
{
public:
	// コンストラクタ
	SNMusicFile();

	// デストラクタ
	virtual ~SNMusicFile();

	// ストリームオープン(非同期実行)
	virtual Void OpenPCMStream();

	// 処理完了？
	virtual Boolean IsOperationComplete();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();
};
