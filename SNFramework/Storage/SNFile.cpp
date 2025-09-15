#include "SNFile.h"
#include "SNStorage.h"
#include "../System/SNWindowsAPI.h"


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
Boolean SNFile::CheckFileExists()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		// 有無確認
		RequestOperation(SNStorageOperationCheckExists);
	}

	// 同期
	else
	{
		// ファイル有無確認
		ret = SNStorage::CheckFileExists(FullFolderFileName.GetString());
	}

	return ret;
}

// ファイルサイズ取得
UInt32 SNFile::GetSize()
{
	UInt32 ret = 0;

	// 非同期
	if (AsyncMode)
	{
		// サイズ取得命令
		RequestOperation(SNStorageOperationGetSize);
	}

	// 同期
	else
	{
		// ファイルサイズ取得
		ret = SNStorage::GetFileSize(FullFolderFileName.GetString());
	}

	return ret;
}

// ファイル書き込み
Boolean SNFile::Write()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		// 書き込み命令
		RequestOperation(SNStorageOperationWriteFile);
	}

	// 同期
	else
	{
		// ファイル書き込み
		ret = SNStorage::WriteFile(FullFolderFileName.GetString(), &FileData);
	}

	return ret;
}

// ファイル読み込み
Boolean SNFile::Read()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		// 読み込み命令
		RequestOperation(SNStorageOperationReadFile);
	}

	// 同期
	else
	{
		// ファイル読み込み
		ret = SNStorage::ReadFile(FullFolderFileName.GetString(), &FileData);
	}

	return ret;
}

// ファイル削除
Boolean SNFile::Delete()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		// 削除命令
		RequestOperation(SNStorageOperationRemoveFile);
	}
	// 同期
	else
	{
		// ファイル削除
		ret = SNStorage::RemoveFile(FullFolderFileName.GetString());
	}

	return ret;

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

// コールバック
Void SNFile::Callback()
{
	SNStorageResult ret = SNStorageResultIdle;

	// 処理中の場合
	if (Result == SNStorageResultProcessing)
	{
		switch (Operation)
		{
		case SNStorageOperationCheckExists:
			// ファイル有無確認
			if (SNStorage::CheckFileExists(FullFolderFileName.GetString()))
			{
				ret = SNStorageResultExists;
			}
			else
			{
				ret = SNStorageResultNoExists;
			}
			break;

		case SNStorageOperationGetSize:
			// ファイルサイズ取得
			FileSize = SNStorage::GetFileSize(FullFolderFileName.GetString());
			ret = SNStorageResultNormal;
			break;

		case SNStorageOperationWriteFile:
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
			break;

		case SNStorageOperationReadFile:
			// サイズ取得
			FileSize = FileData.GetSize();

			// 読み込み
			if (SNStorage::ReadFile(FullFolderFileName.GetString(), &FileData))
			{
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		case SNStorageOperationRemoveFile:
			// ファイル削除
			if (SNStorage::RemoveFile(FullFolderFileName.GetString()))
			{
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		default:
			break;
		}
	}

	// 処理結果をセット
	Result = ret;

	return;
}
