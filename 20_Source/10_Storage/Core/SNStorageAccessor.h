#pragma once
#include "SNFrameworkInternal.h"
#include "SNString.h"


// ストレージアクセッサ
class SNStorageAccessor
{
public:
	// コンストラクタ
	SNStorageAccessor();

	// デストラクタ
	virtual ~SNStorageAccessor();

	// フォルダ設定 (実行ファイル格納位置をRootとした相対パスを設定する)
	virtual Void SetFolderPath(String folder_path);

	// フォルダ/ファイル名設定
	virtual Void SetFolderFileName(String file_name);

	// 非同期処理結果取得
	virtual SNStorageResult GetResult();

	// コールバック
	virtual Void Callback();

	// 命令要求
	virtual Void RequestOperation(SNStorageOperation ope);

	// 命令クリア
	virtual Void ClearOperation();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();

	// 処理完了判定
	virtual Boolean IsOperationComplete();

protected:
	// リクエスト実行関数
	virtual SNStorageResult OnOperationCheckExists();
	virtual SNStorageResult OnOperationCreateFolder();
	virtual SNStorageResult OnOperationRemoveFolder();
	virtual SNStorageResult OnOperationFolderFileList();
	virtual SNStorageResult OnOperationGetSize();
	virtual SNStorageResult OnOperationWriteFile();
	virtual SNStorageResult OnOperationReadFile();
	virtual SNStorageResult OnOperationRemoveFile();

	// フォルダパス
	SNString FolderPath;

	// フォルダ/ファイル名
	SNString FolderFileName;

	// フォルダ/ファイル名(フルパス)
	SNString FullFolderFileName;

	// 非同期処理結果
	volatile SNStorageResult Result;

	// リクエストコマンド
	volatile SNStorageOperation Operation;
};
