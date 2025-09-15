#include "SNFolder.h"
#include "SNStorage.h"
#include "../System/SNWindowsAPI.h"


// フォルダクラス

// コンストラクタ
SNFolder::SNFolder()
{
	FolderList = nullptr;
	FileList = nullptr;
	AllocateList(1, 1);

	Filter.SetString((String)L"*");

	return;
}

// デストラクタ
SNFolder::~SNFolder()
{
	FreeList();

	return;
}

// フォルダ有無チェック
Boolean SNFolder::CheckFolderExists()
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
		// フォルダ有無確認
		ret = SNStorage::CheckFolderExists(FullFolderFileName.GetString());
	}

	return ret;
}

// フォルダ作成
Boolean SNFolder::CreateFolder()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		RequestOperation(SNStorageOperationCreateFolder);
	}
	else
	{
		ret = SNStorage::CreateFolder(FullFolderFileName.GetString());
	}

	return ret;
}

// フォルダ削除
Boolean SNFolder::RemoveFolder()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		RequestOperation(SNStorageOperationRemoveFolder);
	}
	else
	{
		ret = SNStorage::RemoveFolder(FullFolderFileName.GetString());
	}

	return ret;
}

// フォルダ/ファイルリスト生成
Void SNFolder::CreateFolderAndFileList()
{
	// 非同期
	if (AsyncMode)
	{
		// フォルダ/ファイルリスト生成
		RequestOperation(SNStorageOperationFolderFileList);
	}
	// 同期
	else
	{
		// フォルダ数取得
		FolderNum = SNStorage::GetFolderNum(FullFolderFileName.GetString());

		// ファイル数取得
		FileNum = SNStorage::GetFileNum(FullFolderFileName.GetString(), Filter.GetString());

		// リスト用配列確保
		AllocateList(FolderNum, FileNum);

		// フォルダリスト生成
		SNStorage::CreateFolderList(FullFolderFileName.GetString(), FolderList, FolderNum);

		// ファイルリスト生成
		SNStorage::CreateFileList(FullFolderFileName.GetString(), Filter.GetString(), FileList, FileNum);
	}

	return;
}

// サブフォルダ数取得
UInt32 SNFolder::GetFolderNum()
{
	return FolderNum;
}

// サブフォルダ名取得
SNString* SNFolder::GetFolderName(UInt32 index)
{
	return &FolderList[index];
}

// ファイル数取得
UInt32 SNFolder::GetFileNum()
{
	return FileNum;
}

// ファイル名取得
SNString* SNFolder::GetFileName(UInt32 index)
{
	return &FileList[index];
}

// リスト確保
Void SNFolder::AllocateList(UInt32 folder_num, UInt32 file_num)
{
	// リスト解放
	FreeList();

	FolderList = new SNString[folder_num];
	FileList = new SNString[file_num];

	FolderNum = 0;
	FileNum = 0;

	return;
}

// リスト解放
Void SNFolder::FreeList()
{
	if (FolderList != nullptr)
	{
		delete[] FolderList;
		FolderList = nullptr;
		FolderNum = 0;
	}

	if (FileList != nullptr)
	{
		delete[] FileList;
		FileList = nullptr;
		FileNum = 0;
	}

	return;
}

// コールバック
Void SNFolder::Callback()
{
	SNStorageResult ret = SNStorageResultIdle;

	// 処理中の場合
	if (Result == SNStorageResultProcessing)
	{
		switch (Operation)
		{
		case SNStorageOperationCheckExists:
			// フォルダ有無確認
			if (SNStorage::CheckFolderExists(FullFolderFileName.GetString()))
			{
				ret = SNStorageResultExists;
			}
			else
			{
				ret = SNStorageResultNoExists;
			}
			break;

		case SNStorageOperationCreateFolder:
			// フォルダ生成
			if (SNStorage::CreateFolder(FullFolderFileName.GetString()))
			{
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		case SNStorageOperationRemoveFolder:
			// フォルダ削除
			if (SNStorage::RemoveFolder(FullFolderFileName.GetString()))
			{
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		case SNStorageOperationFolderFileList:
			// フォルダ/ファイルリスト生成
			// フォルダ数取得
			FolderNum = SNStorage::GetFolderNum(FullFolderFileName.GetString());

			// ファイル数取得
			FileNum = SNStorage::GetFileNum(FullFolderFileName.GetString(), Filter.GetString());

			// リスト用配列確保
			AllocateList(FolderNum, FileNum);

			// フォルダリスト生成
			SNStorage::CreateFolderList(FullFolderFileName.GetString(), FolderList, FolderNum);

			// ファイルリスト生成
			SNStorage::CreateFileList(FullFolderFileName.GetString(), Filter.GetString(), FileList, FileNum);

			ret = SNStorageResultNormal;

			break;

		default:
			break;
		}
	}

	// 処理結果をセット
	Result = ret;

	return;
}
