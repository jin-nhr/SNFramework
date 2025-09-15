#include "SNApplication.h"
#include "../System/SNWindowsAPI.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNSystem.h"
#include "../Graphics/SNGraphics.h"
#include "../InputDevice/SNInputDevice.h"
#include "SNSoftTimer.h"
#include "SNUserApplication.h"

// アプリケーションクラス

SNThread* SNApplication::ApplicationThread;	// アプリケーションスレッド
SNFPSTimer SNApplication::FPSTimer;		// FPSタイマ
SNFPSCounter SNApplication::FPSCounter;	// FPSカウンター
SNStopWatch SNApplication::ApplicationTimeWatcher;	// アプリケーション処理時間測定
SNLayerController SNApplication::LayerController;		// レイヤ制御
SNDebugLayer		SNApplication::DebugLayer;			// デバッグレイヤ
SNSystemLayer		SNApplication::SystemLayer;			// システムレイヤ
SNApplicationLayer	SNApplication::ApplicationLayer;	// アプリケーションレイヤ
SNBackGroundLayer	SNApplication::BackGroundLayer;		// バックグラウンドレイヤ

SNEvent SNApplication::Event;							// イベントクラス

SNApplicationEventInfo SNApplication::NotifyEvent;		// イベント情報
SNApplicationEventInfo SNApplication::EventSnapshot;	// イベント情報スナップショット


// 初期化処理
Void SNApplication::Initialize()
{
	Int32 cnt = 0;

	// 変数初期化
	NotifyEvent = { 0 };
	EventSnapshot = { 0 };

	// アプリケーションスレッド生成
	ApplicationThread = new SNApplicationThread;

	return;
}

// 起動準備
Void SNApplication::Startup()
{
	// FPS設定
	FPSTimer.SetPFS(SNConfiguration::SystemConfiguration.FPS);

	// ソフトタイマの初期化
	SNSoftTimer::Initialize(SNConfiguration::SystemConfiguration.FPS);

	// レイヤ数設定
	LayerController.SetSceneNum(4);

	// レイヤ設定
	LayerController.SetScene(&DebugLayer);
	LayerController.SetScene(&SystemLayer);
	LayerController.SetScene(&ApplicationLayer);
	LayerController.SetScene(&BackGroundLayer);

	// アプリケーションレイヤ管理を初期化
	// リスト登録後の実行なら、登録したレイヤの初期化も実行される
	LayerController.Initialize();

	// イベント初期化
	Event.Initialize();

	return;
}

// 実行
// リターン：終了コード
Int32 SNApplication::Run()
{
	// スレッドクラスのRunを実行しスレッド生成/起動
	ApplicationThread->Run();

	return 0;
}

// 終了前処理
Void SNApplication::BeforeTerminate()
{
	// ソフトタイマ終了
	SNSoftTimer::Terminate();

	// イベント終了
	Event.Terminate();

	// アプリケーションレイヤ管理終了
	LayerController.Terminate();

	return;
}

// 終了
Void SNApplication::Terminate()
{
	// スレッドクラス破棄
	delete ApplicationThread;

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
	return FPSCounter.GetFPS();
}

// 平均時間取得
UInt32 SNApplication::GetProcTime()
{
	return ApplicationTimeWatcher.GetAverage();
}

// スレッドクラスのユーザー実行関数
Void SNApplication::UserMain()
{
	// FPSタイマを起動
	FPSTimer.Start();

	// FPSカウンター起動
	FPSCounter.Start();

	// レイヤ制御Entry
	LayerController.Entry();

	// メインループ
	while (true)
	{
		// フレーム処理タイミングか確認
		if (FPSTimer.CheckTimeout())
		{
			// 処理時間計測開始
			ApplicationTimeWatcher.Start(false);

			// FPSタイマリスタート
			FPSTimer.Restart();

			// ソフトタイマカウント
			SNSoftTimer::Count();

			// イベントスナップショット
			SnapshotEvent();

			// 入力デバイスチェック
			SNInputDevice::Update();

			// イベント更新
			Event.Update();

			// アプリケーション実行
			LayerController.Step(&Event);

			// FPSの状態によってスキップ判定
			if (!FPSTimer.GetSkipFlag())
			{
				// 描画処理
				LayerController.Draw(SNGraphics::GetSurface());

				// サーフェスフリップ
				SNGraphics::FlipSurface();

				// 画面更新通知
				SNSystem::NoticeRefreshScreen();

				// FPSカウンター更新
				FPSCounter.Count();
			}

			// リザルト = アプリ終了
			if (Event.Result.ExitApplication)
			{
				break;
			}

			//　処理時間計測停止
			ApplicationTimeWatcher.Stop();
		}

		else
		{
			// Sleepする
			FPSTimer.Sleep();
		}
	}

	// レイヤ制御Entry
	LayerController.Exit();

	// 終了通知を送る
	SNSystem::NoticeExitApplication();

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

// アプリケーションスレッドメイン
Void SNApplicationThread::UserMain()
{
	SNApplication::UserMain();
	return;
}
