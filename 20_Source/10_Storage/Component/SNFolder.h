#pragma once
#include "SNFrameworkInternal.h"
#include "SNStorageAccessor.h"
#include "SNList.h"


// フォルダクラス
class SNFolder : public SNStorageAccessor
{
public:
	// コンストラクタ
	SNFolder();

	// デストラクタ
	virtual ~SNFolder();

	// フォルダ有無チェック
	Void CheckFolderExists();

	// フォルダ作成
	Void CreateFolder();

	// フォルダ削除
	Void RemoveFolder();

	// フォルダ/ファイルリスト生成
	Void CreateFolderAndFileList();

	// フォルダ/ファイルリスト解放
	Void ReleaseFolderFileList();

	// サブフォルダ数取得
	UInt32 GetFolderNum();

	// サブフォルダ名取得
	SNString* GetFolderName(UInt32 index);

	// ファイル数取得
	UInt32 GetFileNum();

	// ファイル名取得
	SNString* GetFileName(UInt32 index);

	// リスト確保
	Void AllocateList(UInt32 folder_num, UInt32 file_num);

	// リスト解放
	Void FreeList();

protected:
	virtual SNStorageResult OnOperationCheckExists();
	virtual SNStorageResult OnOperationCreateFolder();
	virtual SNStorageResult OnOperationRemoveFolder();
	virtual SNStorageResult OnOperationFolderFileList();

private:
	UInt32 FolderNum;		// フォルダ数
	SNString* FolderList;	// フォルダリスト
	UInt32 FileNum;			// ファイル数
	SNString* FileList;		// ファイルリスト
	SNString Filter;		// フィルタ
};
