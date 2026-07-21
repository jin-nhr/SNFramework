#include "SNStorage.h"
#include "SNStorageThread.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNAutoResource.h"


// ストレージクラス
SNThread* SNStorage::StorageThread;				// ストレージスレッド
volatile Boolean SNStorage::ThreadEndRequest = false;	// スレッド終了要求

SNString SNStorage::ApplicationPath;			// アプリケーションパス

SNList SNStorage::RequestList;				// リクエストリスト
SNCriticalSection SNStorage::ListAccessCS;	// リストアクセス用クリティカルセクション


// 初期化処理
Void SNStorage::Initialize()
{
	wchar_t path[MAX_PATH];

	// 実行ファイル名(フルパス)を取得
	GetModuleFileName(NULL, path, MAX_PATH);

	// ファイル名を除外
	PathRemoveFileSpec(path);

	// アプリケーションパスをセット
	ApplicationPath.SetString(path);

	// リスト確保
	RequestList.Allocate(SNSystemConfig::StorageRequestMax);

	// リストアクセス用CS
	ListAccessCS.Initialize();
	
	// スレッドクラス生成
	StorageThread = new SNStorageThread;

	return;
}

// 起動準備
Void SNStorage::Startup()
{
	// 他機能でファイルアクセスを必要とするため
	// Startupでスレッドを起動しておく

	// 終了要求をキャンセルしておく
	ThreadEndRequest = false;

	// スレッド実行
	StorageThread->Run();

	return;
}

// 実行
Void SNStorage::Run()
{
	return;
}

// 終了前処理
Void SNStorage::BeforeTerminate()
{
	// 終了要求セット
	ThreadEndRequest = true;

	// 終了待ち(MAX1秒)
	StorageThread->WaitForThreadEnd();

	return;
}

// 終了
Void SNStorage::Terminate()
{
	// スレッドクラス破棄
	delete StorageThread;

	// リスト解放
	RequestList.Free();

	return;
}

// ストレージスレッドメイン
Void SNStorage::UserMain()
{
	SNStorageAccessor* request;

	// ストレージスレッドメインループ
	while (true)
	{
		// 終了要求がセットされたら抜ける
		if (SNStorage::ThreadEndRequest)
		{
			break;
		}

		// リクエスト取得
		request = GetRequest();
		if (request != nullptr)
		{
			request->Callback();
		}

		// 処理後、いったんCPU解放
		::Sleep(1);
	}

	return;
}

// リクエスト
Void SNStorage::Request(SNStorageAccessor* request)
{
	SNListContainer* list;

	{
		SNAutoResource cs(&ListAccessCS);

		// リストの最後にリクエストを追加
		list = RequestList.InsertLast();
		if (list != nullptr)
		{
			list->UserData = (Void*)request;
		}
	}

	return;
}

// リクエスト取得
SNStorageAccessor* SNStorage::GetRequest()
{
	SNListContainer* list;
	SNStorageAccessor* ret = nullptr;

	{
		SNAutoResource cs(&ListAccessCS);

		// リスト先頭からリクエストを取得しリスト削除
		list = RequestList.GetTop();

		if (list != nullptr)
		{
			ret = (SNStorageAccessor*)list->UserData;
			RequestList.RemoveTop();
		}
	}

	return ret;
}

// フォルダ有無確認
Boolean SNStorage::CheckFolderExists(String folder_full)
{
	Boolean ret = false;

	// ファイルorフォルダあり かつ フォルダかどうかを確認
	if (PathFileExists(folder_full) &&
		PathIsDirectory(folder_full))
	{
		ret = true;
	}
		
	return ret;
}

// フォルダ作成
Boolean SNStorage::CreateFolder(String folder_full)
{
	Boolean ret = false;

	// フォルダ有無確認
	if (CheckFolderExists(folder_full))
	{
		ret = true;
	}
	else
	{
		// フォルダなしなら作成
		if (CreateDirectory(folder_full, NULL))
		{
			ret = true;
		}
	}

	return ret;
}

// フォルダ削除
Boolean SNStorage::RemoveFolder(String folder_full)
{
	Boolean ret = false;

	// 削除を試す
	if (::RemoveDirectory(folder_full))
	{
		ret = true;
	}

	return ret;
}

// フォルダ数取得
UInt32 SNStorage::GetFolderNum(String folder_full)
{
	UInt32 count = 0;
	SNString search_path;
	WIN32_FIND_DATAW find_data;
	HANDLE hfind;
	Boolean next;

	// 検索パス（すべてのエントリ）
	search_path.SetString(folder_full);
	search_path.AddString((String)L"\\");
	search_path.AddString((String)L"*");

	// 初回検索
	hfind = ::FindFirstFile(search_path.GetString(), &find_data);

	if (hfind != INVALID_HANDLE_VALUE)
	{
		next = true;

		// 次が検索できる間ループ
		while (next)
		{
			// "." と ".." は除外
			if ((wcscmp(find_data.cFileName, L".") != 0) && (wcscmp(find_data.cFileName, L"..") != 0))
			{
				// ディレクトリならカウント
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					count++;
				}
			}
			// 次検索
			next = (::FindNextFile(hfind, &find_data));
		}

		// ハンドルクローズ
		::FindClose(hfind);
	}

	return count;
}

// ファイル数取得
UInt32 SNStorage::GetFileNum(String folder_full, String filter)
{
	UInt32 count = 0;
	SNString search_path;
	WIN32_FIND_DATAW find_data;
	HANDLE hfind;
	Boolean next;

	// 検索パス（すべてのエントリ）
	search_path.SetString(folder_full);
	search_path.AddString((String)L"\\");
	search_path.AddString(filter);

	// 初回検索
	hfind = ::FindFirstFile(search_path.GetString(), &find_data);

	if (hfind != INVALID_HANDLE_VALUE)
	{
		next = true;

		// 次が検索できる間ループ
		while (next)
		{
			// "." と ".." は除外
			if ((wcscmp(find_data.cFileName, L".") != 0) && (wcscmp(find_data.cFileName, L"..") != 0))
			{
				// ファイルならカウント
				if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					count++;
				}
			}
			// 次検索
			next = (::FindNextFile(hfind, &find_data));
		}

		// ハンドルクローズ
		::FindClose(hfind);
	}

	return count;
}

// フォルダリスト生成
Void SNStorage::CreateFolderList(String folder_full, SNString* folder_list, UInt32 folder_num)
{
	UInt32 count = 0;
	SNString search_path;
	WIN32_FIND_DATAW find_data;
	HANDLE hfind;
	Boolean next;

	// 検索パス（すべてのエントリ）
	search_path.SetString(folder_full);
	search_path.AddString((String)L"\\");
	search_path.AddString((String)L"*");

	// 初回検索
	hfind = ::FindFirstFile(search_path.GetString(), &find_data);

	if (hfind != INVALID_HANDLE_VALUE)
	{
		next = true;

		// 次が検索できる間ループ
		while (next && (count < folder_num))
		{
			// "." と ".." は除外
			if ((wcscmp(find_data.cFileName, L".") != 0) && (wcscmp(find_data.cFileName, L"..") != 0))
			{
				// ディレクトリなら登録
				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// ファイル名を登録
					folder_list[count].SetString((String)find_data.cFileName);
					count++;
				}
			}
			// 次検索
			next = (::FindNextFile(hfind, &find_data));
		}

		// ハンドルクローズ
		::FindClose(hfind);
	}

	return;
}

// ファイルリスト生成
Void SNStorage::CreateFileList(String folder_full, String filter, SNString* file_list, UInt32 file_num)
{
	UInt32 count = 0;
	SNString search_path;
	WIN32_FIND_DATAW find_data;
	HANDLE hfind;
	Boolean next;

	// 検索パス（すべてのエントリ）
	search_path.SetString(folder_full);
	search_path.AddString((String)L"\\");
	search_path.AddString(filter);

	// 初回検索
	hfind = ::FindFirstFile(search_path.GetString(), &find_data);

	if (hfind != INVALID_HANDLE_VALUE)
	{
		next = true;

		// 次が検索できる間ループ
		while (next && (count < file_num))
		{
			// "." と ".." は除外
			if ((wcscmp(find_data.cFileName, L".") != 0) && (wcscmp(find_data.cFileName, L"..") != 0))
			{
				// ファイルならカウント
				if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					// ファイル名を登録
					file_list[count].SetString((String)find_data.cFileName);
					count++;
				}
			}
			// 次検索
			next = (::FindNextFile(hfind, &find_data));
		}

		// ハンドルクローズ
		::FindClose(hfind);
	}

	return;
}


// ファイル有無確認
Boolean SNStorage::CheckFileExists(String file_full)
{
	Boolean ret = false;

	// ファイルorフォルダあり かつ ファイルかどうかを確認
	if (PathFileExists(file_full) &&
		!PathIsDirectory(file_full))
	{
		ret = true;
	}

	return ret;
}

// ファイルサイズ取得
UInt32 SNStorage::GetFileSize(String file_full)
{
	HANDLE file;
	DWORD size = 0;

	// ファイルオープン
	file = CreateFile(file_full, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// オープン成功
	if (file != INVALID_HANDLE_VALUE)
	{
		// サイズ取得
		size = ::GetFileSize(file, NULL);

		// クローズ
		CloseHandle(file);

		// エラー判定
		if ((size == INVALID_FILE_SIZE) && (GetLastError() != NO_ERROR))
		{
			size = 0;
		}
	}

	return size;
}

// ファイル書き込み
Boolean SNStorage::WriteFile(String file_full, SNMemory* write_data)
{
	HANDLE file;
	BOOL result;
	DWORD written = 0;
	Boolean ret = false;
	DWORD size = write_data->GetSize();

	// ファイルオープン
	file = CreateFile(file_full, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// オープン成功
	if (file != INVALID_HANDLE_VALUE)
	{
		// 書き込み
		result = ::WriteFile(file, write_data->GetAddress(), size, &written, NULL);

		// クローズ
		CloseHandle(file);

		// 書き込み正常かつ指定サイズ書けた
		if (result && (written == size))
		{
			ret = true;
		}
	}

	return ret;
}

// ファイル読み込み
Boolean SNStorage::ReadFile(String file_full, SNMemory* read_data)
{
	HANDLE file;
	BOOL result;
	DWORD read = 0;
	Boolean ret = false;
	DWORD size = 0;

	// ファイルオープン
	file = CreateFile(file_full, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// オープン成功
	if (file != INVALID_HANDLE_VALUE)
	{
		// サイズ取得
		size = ::GetFileSize(file, NULL);

		// メモリ確保
		read_data->Allocate(size);

		// 読み込み
		result = ::ReadFile(file, read_data->GetAddress(), size, &read, NULL);

		// クローズ
		CloseHandle(file);

		// 読み込み正常かつ指定サイズ読めた
		if (result && (read == size))
		{
			ret = true;
		}
	}

	return ret;
}

// ファイル削除
Boolean SNStorage::RemoveFile(String file_full)
{
	Boolean ret = false;

	// ファイル削除
	if (::DeleteFileW(file_full))
	{
		ret = true;
	}

	return ret;
}
