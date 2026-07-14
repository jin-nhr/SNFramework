#include "SNFolder.h"
#include "SNStorage.h"
#include "SNWindowsAPI.h"


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
Void SNFolder::CheckFolderExists()
{
	// 有無確認
	RequestOperation(SNStorageOperationCheckExists);
	return;
}

// フォルダ作成
Void SNFolder::CreateFolder()
{
	RequestOperation(SNStorageOperationCreateFolder);
	return;
}

// フォルダ削除
Void SNFolder::RemoveFolder()
{
	RequestOperation(SNStorageOperationRemoveFolder);
	return;
}

// フォルダ/ファイルリスト生成
Void SNFolder::CreateFolderAndFileList()
{
	// フォルダ/ファイルリスト生成
	RequestOperation(SNStorageOperationFolderFileList);
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


SNStorageResult SNFolder::OnOperationCheckExists()
{
	SNStorageResult ret = SNStorageResultIdle;

	// フォルダ有無確認
	if (SNStorage::CheckFolderExists(FullFolderFileName.GetString()))
	{
		ret = SNStorageResultExists;
	}
	else
	{
		ret = SNStorageResultNoExists;
	}

	return ret;
}

SNStorageResult SNFolder::OnOperationCreateFolder()
{
	SNStorageResult ret = SNStorageResultIdle;

	// フォルダ生成
	if (SNStorage::CreateFolder(FullFolderFileName.GetString()))
	{
		ret = SNStorageResultNormal;
	}
	else
	{
		ret = SNStorageResultError;
	}

	return ret;
}

SNStorageResult SNFolder::OnOperationRemoveFolder()
{
	SNStorageResult ret = SNStorageResultIdle;

	// フォルダ削除
	if (SNStorage::RemoveFolder(FullFolderFileName.GetString()))
	{
		ret = SNStorageResultNormal;
	}
	else
	{
		ret = SNStorageResultError;
	}

	return ret;
}

SNStorageResult SNFolder::OnOperationFolderFileList()
{
	SNStorageResult ret = SNStorageResultIdle;

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

	return ret;
}
