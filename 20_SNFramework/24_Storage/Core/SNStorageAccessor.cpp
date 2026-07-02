#include "SNStorageAccessor.h"
#include "SNStorage.h"
#include "SNWindowsAPI.h"

// ストレージアクセッサ

// コンストラクタ
SNStorageAccessor::SNStorageAccessor()
{
	// 変数初期化
	AsyncMode = true;	// デフォルト非同期とする
	Result = SNStorageResultIdle;
	Operation = SNStorageOperationNo;

	return;
}

// デストラクタ
SNStorageAccessor::~SNStorageAccessor()
{
	return;
}

// フォルダ設定 (実行ファイル格納位置をRootとした相対パスを設定する)
Void SNStorageAccessor::SetFolderPath(String folder_path)
{
	Char ch;

	// アプリケーションパス + 相対フォルダ名
	FolderPath.SetString(
		SNStorage::ApplicationPath.GetString(),
		folder_path);

	// 最終文字を取得
	ch = FolderPath.GetString()[FolderPath.GetLength() - 1];

	// 最終文字が\じゃないときは\を付与
	if (ch != L'\\')
	{
		FolderPath.SetString(FolderPath.GetString(), (String)L"\\");
	}

	// フルパスファイル名を構築
	FullFolderFileName.SetString(FolderPath.GetString(), FolderFileName.GetString());

	return;
}

// ファイル名設定
Void SNStorageAccessor::SetFolderFileName(String file_name)
{
	// フォルダパスが未設定の場合はフォルダパス設定
	if (FolderPath.GetLength() <= 0)
	{
		SetFolderPath((String)L"");
	}

	// ファイル名をセット
	FolderFileName.SetString(file_name);

	// フルパスファイル名を構築
	FullFolderFileName.SetString(FolderPath.GetString(), FolderFileName.GetString());

	return;
}

// 非同期モード設定
Void SNStorageAccessor::SetAsyncMode(Boolean async)
{
	// 設定変更の場合
	if (async != AsyncMode)
	{
		// 処理中じゃなければ変更OK
		if (Result != SNStorageResultProcessing)
		{
			// 非同期モード設定
			AsyncMode = async;
			Result = SNStorageResultIdle;
		}
	}

	return;
}

// 非同期処理結果取得
SNStorageResult SNStorageAccessor::GetResult()
{
	return Result;
}

// コールバック
Void SNStorageAccessor::Callback()
{
	return;
}

// 命令要求
Void SNStorageAccessor::RequestOperation(SNStorageOperation ope)
{
	// 命令保存
	Operation = ope;

	// 処理中セット
	Result = SNStorageResultProcessing;

	// ストレージスレッドに対して要求をセット
	SNStorage::Request(this);
	return;
}

// 命令クリア
Void SNStorageAccessor::ClearOperation()
{
	// 命令、結果をクリア
	Operation = SNStorageOperationNo;
	Result = SNStorageResultIdle;

	return;
}
