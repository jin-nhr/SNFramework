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

	// 非同期モード設定
	virtual Void SetAsyncMode(Boolean async);

	// 非同期処理結果取得
	virtual SNStorageResult GetResult();

	// コールバック
	virtual Void Callback();

	// 命令要求
	virtual Void RequestOperation(SNStorageOperation ope);

	// 命令クリア
	virtual Void ClearOperation();

protected:
	// フォルダパス
	SNString FolderPath;

	// フォルダ/ファイル名
	SNString FolderFileName;

	// フォルダ/ファイル名(フルパス)
	SNString FullFolderFileName;

	// 同期モード
	Boolean AsyncMode;

	// 非同期処理結果
	volatile SNStorageResult Result;

	// リクエストコマンド
	volatile SNStorageOperation Operation;
};
