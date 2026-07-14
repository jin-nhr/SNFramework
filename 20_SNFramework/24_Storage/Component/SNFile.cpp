#include "SNFile.h"
#include "SNStorage.h"
#include "SNWindowsAPI.h"


// ファイルクラス

// コンストラクタ
SNFile::SNFile()
{
	// 変数初期化
	FileSize = 0;
	return;
}

// デストラクタ
SNFile::~SNFile()
{
	FileData.Free();
	return;
}

// ファイル有無確認
Void SNFile::CheckFileExists()
{
	// 有無確認
	RequestOperation(SNStorageOperationCheckExists);
	return;
}

// ファイルサイズ取得
Void SNFile::GetDataSize()
{
	// サイズ取得命令
	RequestOperation(SNStorageOperationGetSize);
	return;
}

// ファイル書き込み
Void SNFile::Write()
{
	// 書き込み命令
	RequestOperation(SNStorageOperationWriteFile);
	return;
}

// ファイル読み込み
Void SNFile::Read()
{
	// 読み込み命令
	RequestOperation(SNStorageOperationReadFile);
	return;
}

// ファイル削除
Void SNFile::Delete()
{
	// 削除命令
	RequestOperation(SNStorageOperationRemoveFile);
	return;
}

// データアドレス取得
Void* SNFile::GetDataAddress()
{
	return FileData.GetAddress();
}

// メモリオブジェクト取得
SNMemory* SNFile::GetMemoryObject()
{
	return &FileData;
}

// メモリ解放
Void SNFile::ReleaseMemory()
{
	FileData.Free();
	return;
}

SNStorageResult SNFile::OnOperationCheckExists()
{
	SNStorageResult ret = SNStorageResultIdle;

	// ファイル有無確認
	if (SNStorage::CheckFileExists(FullFolderFileName.GetString()))
	{
		ret = SNStorageResultExists;
	}
	else
	{
		ret = SNStorageResultNoExists;
	}

	return ret;
}

SNStorageResult SNFile::OnOperationGetSize()
{
	SNStorageResult ret = SNStorageResultIdle;

	// ファイルサイズ取得
	FileSize = SNStorage::GetFileSize(FullFolderFileName.GetString());
	ret = SNStorageResultNormal;

	return ret;
}

SNStorageResult SNFile::OnOperationWriteFile()
{
	SNStorageResult ret = SNStorageResultIdle;

	// サイズ取得
	FileSize = FileData.GetSize();

	// 書き込み
	if (SNStorage::WriteFile(FullFolderFileName.GetString(), &FileData))
	{
		ret = SNStorageResultNormal;
	}
	else
	{
		ret = SNStorageResultError;
	}

	return ret;
}

SNStorageResult SNFile::OnOperationReadFile()
{
	SNStorageResult ret = SNStorageResultIdle;

	// 読み込み
	if (SNStorage::ReadFile(FullFolderFileName.GetString(), &FileData))
	{
		// サイズ取得
		FileSize = FileData.GetSize();

		ret = SNStorageResultNormal;
	}
	else
	{
		ret = SNStorageResultError;
	}

	return ret;
}

SNStorageResult SNFile::OnOperationRemoveFile()
{
	SNStorageResult ret = SNStorageResultIdle;

	// ファイル削除
	if (SNStorage::RemoveFile(FullFolderFileName.GetString()))
	{
		FileSize = 0;
		ret = SNStorageResultNormal;
	}
	else
	{
		ret = SNStorageResultError;
	}

	return ret;
}
