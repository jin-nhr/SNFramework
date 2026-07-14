#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"
#include "SNString.h"
#include "SNMemory.h"
#include "SNCriticalSection.h"
#include "SNList.h"
#include "SNStorageAccessor.h"


// ストレージクラス
class SNStorage
{
public:
	// 初期化処理
	static Void Initialize();
	
	// 起動準備
	static Void Startup();
	
	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// スレッドメイン処理
	static Void UserMain();

	// リクエスト
	static Void Request(SNStorageAccessor* request);

	// リクエスト取得
	static SNStorageAccessor* GetRequest();


	///////////////////////////////////////////////
	// オペレーション

	// フォルダ有無確認
	static Boolean CheckFolderExists(String folder_full);

	// フォルダ作成
	static Boolean CreateFolder(String folder_full);

	// フォルダ削除
	static Boolean RemoveFolder(String folder_full);

	// フォルダ数取得
	static UInt32 GetFolderNum(String folder_full);

	// ファイル数取得
	static UInt32 GetFileNum(String folder_full, String filter);

	// フォルダリスト生成
	static Void CreateFolderList(String folder_full, SNString* folder_list, UInt32 folder_num);

	// ファイルリスト生成
	static Void CreateFileList(String folder_full, String filter, SNString* file_list, UInt32 file_num);

	// ファイル有無確認
	static Boolean CheckFileExists(String file_full);

	// ファイルサイズ取得
	static UInt32 GetFileSize(String file_full);

	// ファイル書き込み
	static Boolean WriteFile(String file_full, SNMemory* write_data);

	// ファイル読み込み
	static Boolean ReadFile(String file_full, SNMemory* read_data);

	// ファイル削除
	static Boolean RemoveFile(String file_full);

	///////////////////////////////////////////////

	static SNThread* StorageThread;				// ストレージスレッド
	static volatile Boolean ThreadEndRequest;	// スレッド終了要求

	static SNString ApplicationPath;				// アプリケーションパス

	static SNList RequestList;					// リクエストリスト
	static SNCriticalSection ListAccessCS;		// リストアクセス用クリティカルセクション
};
