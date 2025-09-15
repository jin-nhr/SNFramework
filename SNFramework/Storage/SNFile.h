#pragma once
#include "../Include/SNFramework.h"
#include "SNStorageAccessor.h"
#include "../Library/SNMemory.h"


// ファイルクラス
class SNFile : public SNStorageAccessor
{
public:
	// コンストラクタ
	SNFile();

	// デストラクタ
	virtual ~SNFile();

	// ファイル有無確認
	// true:あり, false:なし
	Boolean CheckFileExists();

	// ファイルサイズ取得
	UInt32 GetSize();

	// ファイル書き込み
	Boolean Write();

	// ファイル読み込み
	Boolean Read();

	// ファイル削除
	Boolean Delete();

	// データアドレス取得
	Void* GetDataAddress();

	// メモリオブジェクト取得
	SNMemory* GetMemoryObject();

	// メモリ解放
	Void ReleaseMemory();

	// コールバック
	virtual Void Callback();

private:
	// メモリ
	SNMemory FileData;

	// サイズ
	UInt32 FileSize;
};
