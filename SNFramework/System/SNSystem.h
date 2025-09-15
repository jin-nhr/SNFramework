#pragma once
#include "../Include/SNFramework.h"
#include "../Configuration/SNConfiguration.h"
#include "../Library/SNMutex.h"
#include "SNWindow.h"

// システムクラス
// SNFrameworkの全コンポーネントの管理を行う
class SNSystem
{
public:
	// 初期化処理
	// パラメータ：WinMainのパラメータを渡す
	static Void Initialize(
		Handle application_handle,
		Handle application_prev_handle,
		String command_line,
		Int32 show_command);
	
	// 起動準備
	static Void Startup();
	
	// 実行
	// リターン：終了コード
	static Int32 Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// 終了通知
	// パラメータ：終了許可(true)/不可(false)
	static Void NoticeExitApplication();

	// 画面更新通知
	static Void NoticeRefreshScreen();

	// アプリケーションハンドル取得
	static Handle GetApplicationHandle();

	// 画面座標→クライアント座標変換
	// パラメータに変換結果を返す
	static Void ScreenToClient(SNPoint* point);

	static Handle	ApplicationHandle;		// アプリケーションハンドル
	static Handle	ApplicationPrevHandle;	// 既存アプリケーションハンドル
	static String	CommandLine;			// コマンドライン
	static Int32	ShowCommand;			// 表示コマンド
	static SNMutex	DualBootChecker;		// 二重起動チェッカー
};

