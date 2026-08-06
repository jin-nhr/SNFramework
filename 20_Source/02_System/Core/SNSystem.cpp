#include "SNSystem.h"
#include "SNConfig.h"
#include "SNWindow.h"
#include "SNStorage.h"
#include "SNGraphics.h"
#include "SNAudioVideo.h"
#include "SNInput.h"
#include "SNApplication.h"
#include "SNGUI.h"
#include "SNCodec.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"


Handle	SNSystem::ApplicationHandle = nullptr;		// アプリケーションハンドル
Handle	SNSystem::ApplicationPrevHandle = nullptr;	// 既存アプリケーションハンドル
String	SNSystem::CommandLine = nullptr;			// コマンドライン
Int32	SNSystem::ShowCommand = 0;					// 表示コマンド
SNMutex	SNSystem::DualBootChecker;					// 二重起動チェッカー


// 初期化処理
Void SNSystem::Initialize(
	Handle application_handle,
	Handle application_prev_handle,
	String command_line,
	Int32 show_command)
{
	// WinMainパラメータを保存
	ApplicationHandle = application_handle;
	ApplicationPrevHandle = application_prev_handle;
	CommandLine = command_line;
	ShowCommand = show_command;

	// システムタイマの解像度向上
	timeBeginPeriod(1);

	// 二重起動チェッカー生成
	DualBootChecker.Create(SNSystemConfig::ApplicationID);

	SNMath::InitTrigFunc();

	// ウインドウ生成
	SNWindow::Create(ApplicationHandle, ShowCommand);

	////////////////////////////////////////////
	// 各コンポーネントのInitializeを実施
	// Initailizeではコンポーネント内の初期化を実施し
	// 他コンポーネントから呼び出されたときに動作できるようにしておく
	SNStorage::Initialize();
	SNConfig::Initialize();
	SNInput::Initialize();
	SNCodec::Initialize();
	SNGraphics::Initialize();
	SNAudioVideo::Initialize();
	SNApplication::Initialize();
	SNGUI::Initialize();

	return;
}

// 起動準備
Void SNSystem::Startup()
{
	////////////////////////////////////////////
	// 各コンポーネントのStartupを実施
	// Startupでは他コンポーネントとの連携などを行い実行準備をする
	SNStorage::Startup();
	SNConfig::Startup();
	SNInput::Startup();
	SNCodec::Startup();
	SNGraphics::Startup();
	SNAudioVideo::Startup();
	SNApplication::Startup();
	SNGUI::Startup();

	return;
}

// 実行
Void SNSystem::Run()
{
	////////////////////////////////////////////
	// 各コンポーネントのRunを実施
	SNStorage::Run();
	SNConfig::Run();
	SNInput::Run();
	SNCodec::Run();
	SNGraphics::Run();
	SNAudioVideo::Run();
	SNApplication::Run();
	SNGUI::Run();

	// システム定常動作を開始
	{
		MSG msg;

		// メイン メッセージ ループ
		// ウインドウが破棄されWM_QUITが発行されるまでループする
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			DispatchMessage(&msg);
		}
	}

	return;
}

// 終了前処理
Void SNSystem::BeforeTerminate()
{
	////////////////////////////////////////////
	// 各コンポーネントのBeforeTerminateを実施
	SNGUI::BeforeTerminate();
	SNApplication::BeforeTerminate();
	SNAudioVideo::BeforeTerminate();
	SNGraphics::BeforeTerminate();
	SNCodec::BeforeTerminate();
	SNInput::BeforeTerminate();
	SNConfig::BeforeTerminate();
	SNStorage::BeforeTerminate();

	return;
}

// 終了
Void SNSystem::Terminate()
{
	////////////////////////////////////////////
	// 各コンポーネントのTerminateを実施
	// 起動時とは逆順に実行する
	SNGUI::Terminate();
	SNApplication::Terminate();
	SNAudioVideo::Terminate();
	SNGraphics::Terminate();
	SNCodec::Terminate();
	SNInput::Terminate();
	SNConfig::Terminate();
	SNStorage::Terminate();

	// システムタイマの解像度を戻す
	timeEndPeriod(1);

	return;
}

// 終了通知
Void SNSystem::NoticeExitApplication()
{
	// Windowに終了通知を送る
	PostMessage((HWND)SNWindow::WindowHandle, WM_SNFRAMEWORK_NOTICE_EXIT, 0, 0);

	return;
}

// アプリケーションハンドル取得
Handle SNSystem::GetApplicationHandle()
{
	return ApplicationHandle;
}

// クライアント領域座標系に変換
Void SNSystem::ScreenToClient(SNPoint* point)
{
	// クライアント座標変換
	// SNPointとPOINTは互換ありのためそのまま渡す
	::ScreenToClient((HWND)SNWindow::WindowHandle, (POINT*)point);

	return;
}

// 多重起動チェック
Boolean SNSystem::CheckDuplicateInstance()
{
	Boolean conf = SNSystemConfig::SingleApp;		// 単一起動のみ許可ならtrue
	Boolean first_boot = DualBootChecker.IsFirst();	// 単一起動ならtrue
	Boolean ret = false;

	// 単一起動のみ許可で、最初のインスタンスではない
	if (conf && !first_boot)
	{
		ret = true;
	}

	return ret;
}
