#include "SNStorageAccessor.h"
#include "SNStorage.h"
#include "SNWindowsAPI.h"
#include "SNSystemTimer.h"
#include "SNSystemConfig.h"

// ストレージアクセッサ

// コンストラクタ
SNStorageAccessor::SNStorageAccessor()
{
	// 変数初期化
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

// 非同期処理結果取得
SNStorageResult SNStorageAccessor::GetResult()
{
	return Result;
}

// コールバック
Void SNStorageAccessor::Callback()
{
	SNStorageResult ret = SNStorageResultIdle;

	// 処理中のときだけ実行
	if (Result == SNStorageResultProcessing)
	{
		switch (Operation)
		{
		case SNStorageOperationCheckExists:
			ret = OnOperationCheckExists();
			break;
		case SNStorageOperationCreateFolder:
			ret = OnOperationCreateFolder();
			break;
		case SNStorageOperationRemoveFolder:
			ret = OnOperationRemoveFolder();
			break;
		case SNStorageOperationFolderFileList:
			ret = OnOperationFolderFileList();
			break;
		case SNStorageOperationGetSize:
			ret = OnOperationGetSize();
			break;
		case SNStorageOperationWriteFile:
			ret = OnOperationWriteFile();
			break;
		case SNStorageOperationReadFile:
			ret = OnOperationReadFile();
			break;
		case SNStorageOperationRemoveFile:
			ret = OnOperationRemoveFile();
			break;
		}
	}

	Result = ret;

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

// 処理完了待ち
Void SNStorageAccessor::WaitForOperationComplete()
{
	SNSystemTimer timer(SNSystemConfig::StorageTimeout);

	timer.Start();

	while ((!IsOperationComplete()) && (!timer.CheckTimeout()))
	{
		::Sleep(1);
	}

	return;
}

Boolean SNStorageAccessor::IsOperationComplete()
{
	return (GetResult() != SNStorageResultProcessing);
}

SNStorageResult SNStorageAccessor::OnOperationCheckExists()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationCreateFolder()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationRemoveFolder()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationFolderFileList()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationGetSize()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationWriteFile()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationReadFile()
{
	return SNStorageResultIdle;
}

SNStorageResult SNStorageAccessor::OnOperationRemoveFile()
{
	return SNStorageResultIdle;
}
