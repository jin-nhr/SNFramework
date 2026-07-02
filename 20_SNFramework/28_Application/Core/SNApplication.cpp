#include "SNApplication.h"
#include "SNApplicationThread.h"
#include "SNEvent.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNGraphics.h"
#include "SNInput.h"
#include "SNSoftTimer.h"
#include "SNAutoResource.h"

// アプリケーションクラス

SNThread* SNApplication::ApplicationThread;	// アプリケーションスレッド
SNFPSTimer SNApplication::FPSTimer;		// FPSタイマ
SNFPSCounter SNApplication::FPSCounter;	// FPSカウンター
SNStopWatch SNApplication::ApplicationTimeWatcher;	// アプリケーション処理時間測定

///////////////////////////////////////////////////////////

// Frameworkアプリレイヤ管理
SNLayerController SNApplication::FrameworkAppLayer;

// デバッグアプリ
SNStateController SNApplication::DebugAppStateCtrl;	// 状態管理
SNDebugAppOff SNApplication::DebugAppOff;
SNDebugAppRun SNApplication::DebugAppRun;

// システムアプリ
SNStateController SNApplication::SysAppStateCtrl;	// 状態管理
SNSysAppPreStart SNApplication::SysAppPreStart;
SNSysAppStartup SNApplication::SysAppStartup;
SNSysAppNoApp SNApplication::SysAppNoApp;
SNSysAppIdle SNApplication::SysAppIdle;
SNSysAppScreenKeyboard SNApplication::SysAppScreenKeyboard;
SNSysAppScreenGamePad SNApplication::SysAppScreenGamePad;
SNSysAppConfig SNApplication::SysAppConfig;
SNSysAppInputConfig SNApplication::SysAppInputConfig;
SNSysAppPreEnd SNApplication::SysAppPreEnd;

// ユーザーアプリ
SNStateController SNApplication::UserAppStateCtrl;	// 状態管理
SNUserAppOff SNApplication::UserAppOff;

// バックグラウンドアプリ
SNBgApp SNApplication::BgApp;

///////////////////////////////////////////////////////////

volatile Boolean SNApplication::NotifyEventInfo[SNEventNum];	// イベント情報
Boolean SNApplication::EventSnapshot[SNEventNum];				// イベント情報スナップショット

// 初期化処理
Void SNApplication::Initialize()
{
	Int32 cnt = 0;

	// 変数初期化
	for (cnt = 0; cnt < SNApplicationEvent::SNEventNum; cnt++)
	{
		NotifyEventInfo[cnt] = false;
		EventSnapshot[cnt] = false;
	}

	// アプリケーションスレッド生成
	ApplicationThread = new SNApplicationThread;

	return;
}

// 起動準備
Void SNApplication::Startup()
{
	// ソフトタイマの初期化
	SNSoftTimer::Initialize();

	// デバッグアプリ設定
	DebugAppStateCtrl.SetSceneNum(2);
	DebugAppStateCtrl.SetScene(&DebugAppOff,  1, -1, -1, -1);
	DebugAppStateCtrl.SetScene(&DebugAppRun, -1, -1, -1, -1);

	// システムアプリ設定
	SysAppStateCtrl.SetSceneNum(9);
	SysAppStateCtrl.SetScene(&SysAppPreStart, 1, -1, -1, -1);		// 0
	SysAppStateCtrl.SetScene(&SysAppStartup,  3, 2, -1, -1);		// 1
	SysAppStateCtrl.SetScene(&SysAppNoApp, -1, -1, -1, -1);			// 2
	SysAppStateCtrl.SetScene(&SysAppIdle, 4, 5, 6, 8);			// 3
	SysAppStateCtrl.SetScene(&SysAppScreenKeyboard, 3, -1, -1, -1);// 4
	SysAppStateCtrl.SetScene(&SysAppScreenGamePad, 3, -1, -1, 8); // 5
	SysAppStateCtrl.SetScene(&SysAppConfig, 3, 7, -1, 8);		// 6
	SysAppStateCtrl.SetScene(&SysAppInputConfig, 6, -1, -1, 8);	// 7
	SysAppStateCtrl.SetScene(&SysAppPreEnd, 3, -1, -1, -1);		// 8

	// ユーザーアプリ設定
	UserAppStateCtrl.SetSceneNum(2);
	UserAppStateCtrl.SetScene(&UserAppOff, 1, -1, -1, -1);
	// ユーザーアプリあり
	if (SNUserAppBase::UserApplication != nullptr)
	{
		// ユーザーアプリ登録
		UserAppStateCtrl.SetScene(SNUserAppBase::UserApplication, -1, -1, -1, -1);
	}
	// ユーザーアプリなし
	else
	{
		// 何か登録しておきたいのでOFFを登録しておく
		UserAppStateCtrl.SetScene(&UserAppOff, 0, 0, 0, 0);
	}

	// バックグラウンドアプリ設定
	// 単一シーン構成のため特に設定なし

	// アプリケーションレイヤ設定
	FrameworkAppLayer.SetSceneNum(4);
	FrameworkAppLayer.SetScene(&DebugAppStateCtrl);
	FrameworkAppLayer.SetScene(&SysAppStateCtrl);
	FrameworkAppLayer.SetScene(&UserAppStateCtrl);
	FrameworkAppLayer.SetScene(&BgApp);

	// アプリケーションレイヤ管理を初期化
	// リスト登録後の実行なら、登録したレイヤの初期化も実行される
	FrameworkAppLayer.Initialize();

	// イベント初期化
	SNEvent::Initialize();

	return;
}

// 実行
Void SNApplication::Run()
{
	// スレッドクラスのRunを実行しスレッド生成/起動
	ApplicationThread->Run();

	return;
}

// 終了前処理
Void SNApplication::BeforeTerminate()
{
	// ソフトタイマ終了
	SNSoftTimer::Terminate();

	// イベント終了
	SNEvent::Terminate();

	// アプリケーションレイヤ管理終了
	FrameworkAppLayer.Terminate();

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
Boolean SNApplication::GetEventInfo(SNApplicationEvent event)
{
	return EventSnapshot[event];
}

// イベント通知
Void SNApplication::NotifyEvent(SNApplicationEvent event)
{
	NotifyEventInfo[event] = true;

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
	FrameworkAppLayer.Entry();

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
			SNInput::Update();

			// イベント更新
			SNEvent::Update();

			// アプリケーション実行
			FrameworkAppLayer.Step();

			// FPSの状態によってスキップ判定
			if (!FPSTimer.GetSkipFlag())
			{
				// 描画処理
				FrameworkAppLayer.Draw(SNGraphics::GetSurface());

				// サーフェスフリップ
				SNGraphics::FlipSurface();

				// 画面更新通知
				SNSystem::NoticeRefreshScreen();

				// FPSカウンター更新
				FPSCounter.Count();
			}

			// リザルト処理 (falseなら終了)
			if (!EventResultProc())
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

	// レイヤ制御Exit
	FrameworkAppLayer.Exit();

	// 終了通知を送る
	SNSystem::NoticeExitApplication();

	return;
}


// イベントスナップショット
Void SNApplication::SnapshotEvent()
{
	Int32 cnt = 0;

	// 外部から通知されるイベント情報を
	// アプリケーションスレッドで参照するための
	// スナップショットを作成する
	// 通知があるものについては参照後クリアを行う
	// 排他制御を行わないためtrueを確認したもののみfalse設定する

	for (cnt = 0; cnt < SNApplicationEvent::SNEventNum; cnt++)
	{
		// イベントの通知あり
		if (NotifyEventInfo[cnt])
		{
			EventSnapshot[cnt] = true;
			NotifyEventInfo[cnt] = false;
		}

		// 通知なし
		else
		{
			EventSnapshot[cnt] = false;
		}
	}
	
	return;
}

// リザルト処理
// リターン： true:継続、false:終了
Boolean SNApplication::EventResultProc()
{
	Boolean ret = true;

	// アプリ終了
	if (SNEvent::EventResult[SNEventResultExitApplication])
	{
		ret = false;
	}
	else
	{
		// その他イベント処理
	}

	return ret;
}

