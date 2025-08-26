#pragma once
#include "../Include/SNFramework.h"
#include "../Configuration/SNConfiguration.h"
#include "../Library/SNMutex.h"
#include "SNWindow.h"

// システムクラス
// SNFrameworkの全コンポーネントの管理を行う
class SNSystem
{
// 共通メソッド/データ
public:
	// インスタンス生成/取得
	static SNSystem* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	// 自身のインスタンス
	static SNSystem* Me;


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNSystem();
	
	// 初期化処理
	// パラメータ：WinMainのパラメータを渡す
	Void Initialize(
		Handle application_handle,
		Handle application_prev_handle,
		String command_line,
		Int32 show_command);
	
	// 起動準備
	Void Startup();
	
	// 実行
	// リターン：終了コード
	Int32 Run();

	// 終了前処理
	Void BeforeTerminate();
	
	// 終了
	Void Terminate();

	// 終了通知
	// パラメータ：終了許可(true)/不可(false)
	Void NoticeExitApplication();

	// 画面更新通知
	Void NoticeRefreshScreen();

	// アプリケーションハンドル取得
	Handle	GetApplicationHandle();

	// ウインドウDC取得
	Handle GetWindowDC();

	// 画面座標→クライアント座標変換
	// パラメータに変換結果を返す
	Void ScreenToClient(SNPoint* point);

private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNSystem();

	Handle	ApplicationHandle;		// アプリケーションハンドル
	Handle  ApplicationPrevHandle;	// 既存アプリケーションハンドル
	String	CommandLine;			// コマンドライン
	Int32	ShowCommand;			// 表示コマンド
	SNMutex	DualBootChecker;		// 二重起動チェッカー
	SNWindow Window;				// ウインドウ
};

