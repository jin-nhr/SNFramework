#include "SNSystem.h"
#include "../Configuration/SNConfiguration.h"
#include "../Storage/SNStorage.h"
#include "../Graphics/SNGraphics.h"
#include "../AudioVideo/SNAudioVideo.h"
#include "../InputDevice/SNInputDevice.h"
#include "../Application/SNApplication.h"
#include "SNWindowsAPI.h"

// システムクラス

Handle	SNSystem::ApplicationHandle = nullptr;		// アプリケーションハンドル
Handle	SNSystem::ApplicationPrevHandle = nullptr;	// 既存アプリケーションハンドル
String	SNSystem::CommandLine = nullptr;			// コマンドライン
Int32	SNSystem::ShowCommand = 0;					// 表示コマンド
SNMutex	SNSystem::DualBootChecker;					// 二重起動チェッカー


// 初期化処理
// パラメータ：WinMainのパラメータを渡す
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

	////////////////////////////////////////////
	// 各コンポーネントのInitializeを実施
	// Initailizeではコンポーネント内の初期化を実施し
	// 他コンポーネントから呼び出されたときに動作できるようにしておく
	SNConfiguration::Initialize();
	SNStorage::Initialize();
	SNGraphics::Initialize();
	SNAudioVideo::Initialize();
	SNInputDevice::Initialize();
	SNApplication::Initialize();

	return;
}

// 起動準備
Void SNSystem::Startup()
{
	// 二重起動チェッカー生成
	DualBootChecker.Create(SNConfiguration::SystemConfiguration.ApplicationID);

	// ウインドウ生成
	SNWindow::Create(ApplicationHandle, ShowCommand);

	////////////////////////////////////////////
	// 各コンポーネントのStartupを実施
	// Startupでは他コンポーネントとの連携などを行い実行準備をする
	SNConfiguration::Startup();
	SNStorage::Startup();
	SNGraphics::Startup();
	SNAudioVideo::Startup();
	SNInputDevice::Startup();
	SNApplication::Startup();

	return;
}

// 実行
// リターン：終了コード
Int32 SNSystem::Run()
{
	////////////////////////////////////////////
	// 各コンポーネントのRunを実施
	SNConfiguration::Run();
	SNStorage::Run();
	SNGraphics::Run();
	SNAudioVideo::Run();
	SNInputDevice::Run();
	SNApplication::Run();


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

	return 0;
}

// 終了前処理
Void SNSystem::BeforeTerminate()
{
	////////////////////////////////////////////
	// 各コンポーネントのBeforeTerminateを実施
	// 起動時とは逆順に実行する
	SNApplication::BeforeTerminate();
	SNStorage::BeforeTerminate();
	SNGraphics::BeforeTerminate();
	SNAudioVideo::BeforeTerminate();
	SNInputDevice::BeforeTerminate();
	SNConfiguration::BeforeTerminate();

	return;
}

// 終了
Void SNSystem::Terminate()
{
	// システムタイマの解像度を戻す
	timeEndPeriod(1);

	////////////////////////////////////////////
	// 各コンポーネントのTerminateを実施
	// 起動時とは逆順に実行する
	SNApplication::Terminate();
	SNStorage::Terminate();
	SNGraphics::Terminate();
	SNAudioVideo::Terminate();
	SNInputDevice::Terminate();
	SNConfiguration::Terminate();

	return;
}

// 終了通知
// パラメータ：終了許可(true)/不可(false)
Void SNSystem::NoticeExitApplication()
{
	// Windowに終了通知を送る
	PostMessage((HWND)SNWindow::WindowHandle, WM_SNFRAMEWORK_NOTICE_EXIT, 0, 0);

	return;
}

// 画面更新通知
Void SNSystem::NoticeRefreshScreen()
{
	// 画面更新通知
	SNWindow::EnableUpdate = true;
	InvalidateRect((HWND)SNWindow::WindowHandle, nullptr, FALSE);

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
