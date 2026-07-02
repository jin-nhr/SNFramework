#pragma once
#include "SNFrameworkInternal.h"
#include "SNStorageAccessor.h"
#include "SNMemory.h"


// ファイルクラス
class SNFile : public SNStorageAccessor
{
public:
	// コンストラクタ
	SNFile();

	// デストラクタ
	virtual ~SNFile();

	// ファイル有無確認
	// 同期モードではリターンがtrue：ファイルあり/ false：ファイルなし
	// 非同期モードでは非同期処理の結果を参照すること
	Boolean CheckFileExists();

	// ファイルサイズ取得
	// 同期モードではサイズをリターン
	// 非同期モードでは常に0を返し非同期処理完了後にFileSizeを直接参照すること
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

	// サイズ
	// 同期モードではGetSizeを使うこと
	// 非同期モードではサイズ取得完了後に参照すること
	UInt32 FileSize;

private:
	// メモリ
	SNMemory FileData;
};
