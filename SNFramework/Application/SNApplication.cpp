#include "SNApplication.h"
#include "../System/SNWindowsAPI.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNSystem.h"
#include "../Graphics/SNGraphics.h"
#include "../InputDevice/SNInputDevice.h"
#include "SNSoftTimer.h"
#include "SNUserApplication.h"

// アプリケーションクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNApplication* SNApplication::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNApplication;
	}

	return Me;
}

// インスタンス破棄
Void SNApplication::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}


// 自身のインスタンス
SNApplication* SNApplication::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNApplication::~SNApplication()
{
	return;
}


// 初期化処理
Void SNApplication::Initialize()
{
	// 各クラスインスタンス生成
	FPSTimer = new SNFPSTimer;
	FPSCounter = new SNFPSCounter;
	ApplicationTimeWatcher = new SNStopWatch;
	ApplicationLayerManager = new SNLayerManager;
	DebugLayer = new SNDebugLayer;
	SystemLayer = new SNSystemLayer;
	ApplicationLayer = new SNApplicationLayer;
	BackGroundLayer = new SNBackGroundLayer;
	Event = new SNEvent;

	return;
}

// 起動準備
Void SNApplication::Startup()
{
	// コンフィギュレーション取得
	SNConfigurationData* configuration = &SNConfiguration::GetInstance()->ConfigurationData;

	// FPS設定
	FPSTimer->SetPFS(configuration->System.FPS);

	// ソフトタイマの初期化
	SNSoftTimer::Initialize(configuration->System.FPS);

	// レイヤリスト構成
	ApplicationLayerList[0] = DebugLayer;
	ApplicationLayerList[1] = SystemLayer;

	// ユーザーアプリケーション未登録
	if (SNUserApplication::UserApplication == nullptr)
	{
		ApplicationLayerList[2] = ApplicationLayer;
	}
	// ユーザーアプリケーション登録済み
	else
	{
		ApplicationLayerList[2] = SNUserApplication::UserApplication;
	}
	ApplicationLayerList[3] = BackGroundLayer;

	// レイヤリストをセット
	ApplicationLayerManager->SetLayerInfo(ApplicationLayerNum, ApplicationLayerList);

	// アプリケーションレイヤ管理を初期化
	// リスト登録後の実行なら、登録したレイヤの初期化も実行される
	ApplicationLayerManager->Initialize();

	// イベント初期化
	Event->Initialize();

	return;
}

// 実行
// リターン：終了コード
Int32 SNApplication::Run()
{
	// スレッドクラスのRunを実行しスレッド生成/起動
	SNThread::Run();

	return 0;
}

// 終了前処理
Void SNApplication::BeforeTerminate()
{
	// ソフトタイマ終了
	SNSoftTimer::Terminate();

	// イベント終了
	Event->Terminate();

	// アプリケーションレイヤ管理終了
	ApplicationLayerManager->Terminate();

	return;
}

// 終了
Void SNApplication::Terminate()
{
	// インスタンス破棄
	delete ApplicationLayerManager;	// 子レイヤ破棄前にManager破棄

	delete FPSTimer;
	delete FPSCounter;
	delete ApplicationTimeWatcher;
	delete DebugLayer;
	delete SystemLayer;
	delete ApplicationLayer;
	delete BackGroundLayer;
	delete Event;

	return;
}

// イベント情報取得
SNApplicationEventInfo* SNApplication::GetEventInfo()
{
	return &EventSnapshot;
}

// アクティブ通知
Void SNApplication::NotifyActive()
{
	NotifyEvent.Active = true;

	return;
}

// 非アクティブ通知
Void SNApplication::NotifyNonActive()
{
	NotifyEvent.NonActive = true;

	return;
}

// 終了通知
Void SNApplication::NotifyExitApplication()
{
	NotifyEvent.ExitApplication = true;

	return;
}

// ホイールUp通知
Void SNApplication::NotifyWheelUp()
{
	NotifyEvent.WheelUp = true;

	return;
}

// ホイールDown通知
Void SNApplication::NotifyWheelDown()
{
	NotifyEvent.WheelDown = true;

	return;
}

// FPS取得
UInt32 SNApplication::GetFPS()
{
	return FPSCounter->GetFPS();
}

// 平均時間取得
UInt32 SNApplication::GetProcTime()
{
	return ApplicationTimeWatcher->GetAverage();
}



// コンストラクタ
// 外部からのインスタンス生成は禁止
SNApplication::SNApplication() : SNThread()
{
	Int32 cnt = 0;

	// 変数初期化
	NotifyEvent = {0};
	EventSnapshot = {0};

	for (cnt = 0; cnt < ApplicationLayerNum; cnt++)
	{
		ApplicationLayerList[cnt] = nullptr;
	}

	FPSTimer = nullptr;
	FPSCounter = nullptr;
	ApplicationTimeWatcher = nullptr;
	ApplicationLayerManager = nullptr;
	DebugLayer = nullptr;
	SystemLayer = nullptr;
	ApplicationLayer = nullptr;
	BackGroundLayer = nullptr;
	Event = nullptr;

	return;
}


// スレッドクラスのユーザー実行関数
Void SNApplication::UserMain()
{
	SNGraphics* graphics = SNGraphics::GetInstance();

	// FPSタイマを起動
	FPSTimer->Start();

	// FPSカウンター起動
	FPSCounter->Start();

	// アプリケーションレイヤ管理Entry
	ApplicationLayerManager->Entry();

	// メインループ
	while (true)
	{
		// フレーム処理タイミングか確認
		if (FPSTimer->CheckTimeout())
		{
			// 処理時間計測開始
			ApplicationTimeWatcher->Start(false);

			// FPSタイマリスタート
			FPSTimer->Restart();

			// ソフトタイマカウント
			SNSoftTimer::Count();

			// FPSの状態によってスキップ判定
			if (!FPSTimer->GetSkipFlag())
			{
				// イベントスナップショット
				SnapshotEvent();

				// 入力デバイスチェック
				SNInputDevice::GetInstance()->Update();

				// イベント更新
				Event->Update();

				// アプリケーション実行
				ApplicationLayerManager->Step(Event);

				// 描画処理
				ApplicationLayerManager->Draw(graphics->GetSurface());

				// サーフェスフリップ
				graphics->FlipSurface();

				// 画面更新通知
				SNSystem::GetInstance()->NoticeRefreshScreen();

				// FPSカウンター更新
				FPSCounter->Count();
			}

			// リザルト = アプリ終了
			if (Event->Result.ExitApplication)
			{
				break;
			}

			//　処理時間計測停止
			ApplicationTimeWatcher->Stop();
		}

		else
		{
			// Sleepする
			FPSTimer->Sleep();
		}
	}

	// アプリケーションレイヤ管理EExit
	ApplicationLayerManager->Exit();

	// 終了通知を送る
	SNSystem::GetInstance()->NoticeExitApplication();

	return;
}


// イベントスナップショット
Void SNApplication::SnapshotEvent()
{
	// 外部から通知されるイベント情報を
	// アプリケーションスレッドで参照するための
	// スナップショットを作成する
	// 通知があるものについては参照後クリアを行う
	// 排他制御を行わないためtrueを確認したもののみfalse設定する

	// スナップショットを事前にクリア
	EventSnapshot = {0};

	// アクティブ通知
	if (NotifyEvent.Active)
	{
		EventSnapshot.Active = true;
		NotifyEvent.Active = false;
	}
	
	// 非アクティブ通知
	if (NotifyEvent.NonActive)
	{
		EventSnapshot.NonActive = true;
		NotifyEvent.NonActive = false;
	}
	
	// 終了通知
	if (NotifyEvent.ExitApplication)
	{
		EventSnapshot.ExitApplication = true;
		NotifyEvent.ExitApplication = false;
	}
	
	// ホイールUp
	if (NotifyEvent.WheelUp)
	{
		EventSnapshot.WheelUp = true;
		NotifyEvent.WheelUp = false;
	}
	
	// ホイールDown
	if (NotifyEvent.WheelDown)
	{
		EventSnapshot.WheelDown = true;
		NotifyEvent.WheelDown = false;
	}

	return;
}
