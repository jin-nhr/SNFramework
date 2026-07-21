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
	Void CheckFileExists();

	// ファイルサイズ取得
	Void GetDataSize();

	// ファイル書き込み
	Void Write();

	// ファイル読み込み
	Void Read();

	// ファイル削除
	Void Delete();

	// データアドレス取得
	Void* GetDataAddress();

	// メモリオブジェクト取得
	SNMemory* GetMemoryObject();

	// メモリ解放
	Void ReleaseMemory();

	// サイズ
	// Read/Write/GetSize後に参照すること
	UInt32 FileSize;

protected:
	virtual SNStorageResult OnOperationCheckExists();
	virtual SNStorageResult OnOperationGetSize();
	virtual SNStorageResult OnOperationWriteFile();
	virtual SNStorageResult OnOperationReadFile();
	virtual SNStorageResult OnOperationRemoveFile();

private:
	// メモリ
	SNMemory FileData;
};
