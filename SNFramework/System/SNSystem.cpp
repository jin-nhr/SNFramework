#include "SNSystem.h"
#include "../Configuration/SNConfiguration.h"
#include "../Storage/SNStorage.h"
#include "../Graphics/SNGraphics.h"
#include "../AudioVideo/SNAudioVideo.h"
#include "../InputDevice/SNInputDevice.h"
#include "../Application/SNApplication.h"
#include "SNWindowsAPI.h"

// システムクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNSystem* SNSystem::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNSystem;
	}

	return Me;
}

// インスタンス破棄
Void SNSystem::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNSystem* SNSystem::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNSystem::~SNSystem()
{

	return;
}


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
	SNConfiguration::GetInstance()->Initialize();
	SNStorage::GetInstance()->Initialize();
	SNGraphics::GetInstance()->Initialize();
	SNAudioVideo::GetInstance()->Initialize();
	SNInputDevice::GetInstance()->Initialize();
	SNApplication::GetInstance()->Initialize();

	return;
}

// 起動準備
Void SNSystem::Startup()
{
	// 二重起動チェッカー生成
	DualBootChecker.Create(SNConfiguration::GetInstance()->ConfigurationData.System.ApplicationID);

	// ウインドウ生成
	Window.Create(ApplicationHandle, ShowCommand);

	////////////////////////////////////////////
	// 各コンポーネントのStartupを実施
	// Startupでは他コンポーネントとの連携などを行い実行準備をする
	SNConfiguration::GetInstance()->Startup();
	SNStorage::GetInstance()->Startup();
	SNGraphics::GetInstance()->Startup();
	SNAudioVideo::GetInstance()->Startup();
	SNInputDevice::GetInstance()->Startup();
	SNApplication::GetInstance()->Startup();

	return;
}

// 実行
// リターン：終了コード
Int32 SNSystem::Run()
{
	////////////////////////////////////////////
	// 各コンポーネントのRunを実施
	SNConfiguration::GetInstance()->Run();
	SNStorage::GetInstance()->Run();
	SNGraphics::GetInstance()->Run();
	SNAudioVideo::GetInstance()->Run();
	SNInputDevice::GetInstance()->Run();
	SNApplication::GetInstance()->Run();


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
	SNApplication::GetInstance()->BeforeTerminate();
	SNStorage::GetInstance()->BeforeTerminate();
	SNGraphics::GetInstance()->BeforeTerminate();
	SNAudioVideo::GetInstance()->BeforeTerminate();
	SNInputDevice::GetInstance()->BeforeTerminate();
	SNConfiguration::GetInstance()->BeforeTerminate();

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
	SNApplication::GetInstance()->Terminate();
	SNStorage::GetInstance()->Terminate();
	SNGraphics::GetInstance()->Terminate();
	SNAudioVideo::GetInstance()->Terminate();
	SNInputDevice::GetInstance()->Terminate();
	SNConfiguration::GetInstance()->Terminate();

	return;
}

// 終了通知
// パラメータ：終了許可(true)/不可(false)
Void SNSystem::NoticeExitApplication(Boolean permission)
{
	// Windowに終了通知を送る
	PostMessage((HWND)Window.GetWindowHandle(), WM_SNFRAMEWORK_NOTICE_EXIT, (WPARAM)permission, 0);

	return;
}

// 画面更新通知
Void SNSystem::NoticeRefreshScreen()
{
	// 画面更新通知
	PostMessage((HWND)Window.GetWindowHandle(), WM_SNFRAMEWORK_NOTICE_REFRESHSCREEN, 0, 0);

	return;
}

// アプリケーションハンドル取得
Handle SNSystem::GetApplicationHandle()
{
	return ApplicationHandle;
}

// ウインドウDC取得
Handle SNSystem::GetWindowDC()
{
	// ウインドウDCを返す
	return Window.GetClientDC();
}

// コンストラクタ
// 外部からのインスタンス生成は禁止
SNSystem::SNSystem()
{
	// インスタンス所有の変数初期化
	ApplicationHandle = nullptr;
	ApplicationPrevHandle = nullptr;
	CommandLine = nullptr;
	ShowCommand = 0;

	return;
}

