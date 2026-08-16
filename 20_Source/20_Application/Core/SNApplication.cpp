#include "SNApplication.h"
#include "SNApplicationThread.h"
#include "SNEvent.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNGraphics.h"
#include "SNInput.h"
#include "SNSoftTimer.h"
#include "SNAutoResource.h"
#include "SNAudioVideo.h"

// アプリケーションクラス

Boolean SNApplication::Active = true;

SNThread* SNApplication::ApplicationThread;	// アプリケーションスレッド
SNFPSTimer SNApplication::FPSTimer;		// FPSタイマ
SNFPSCounter SNApplication::FPSCounter;	// FPSカウンター
SNFPSCounter SNApplication::ProcFPSCounter;	// 処理FPSカウンター
SNFPSCounter SNApplication::FrameSkipCounter; /// フレームスキップカウンター
SNStopWatch SNApplication::ApplicationTimeWatcher;	// アプリケーション処理時間測定

///////////////////////////////////////////////////////////

// Frameworkアプリレイヤ管理
SNLayerController SNApplication::FrameworkAppLayer;

// アプリ定義
SNDebugApp SNApplication::DebugApp;
SNOverlapApp SNApplication::OverlapApp;
SNSysApp SNApplication::SysApp;
SNUserApp SNApplication::UserApp;
SNWorldStateMng SNApplication::World;
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

	// アプリケーションレイヤ設定
	FrameworkAppLayer.SetSceneNum(6);
	FrameworkAppLayer.SetScene(&DebugApp);
	FrameworkAppLayer.SetScene(&OverlapApp);
	FrameworkAppLayer.SetScene(&SysApp);
	FrameworkAppLayer.SetScene(&UserApp);
	FrameworkAppLayer.SetScene(&World);
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

// 処理FPS取得
UInt32 SNApplication::GetProcFPS()
{
	return ProcFPSCounter.GetFPS();
}

UInt32 SNApplication::GetSkipFrame()
{
	return FrameSkipCounter.GetFPS();
}

// 平均時間取得
UInt32 SNApplication::GetProcTime()
{
	return ApplicationTimeWatcher.GetAverage();
}

// スレッドクラスのユーザー実行関数
Void SNApplication::UserMain()
{
	SNGraphicsContext* grc;

	// FPSタイマを起動
	FPSTimer.Start();

	// FPSカウンター起動
	FPSCounter.Start();
	ProcFPSCounter.Start();

	// フレームスキップカウンター
	FrameSkipCounter.Start();

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

			// 状態更新
			Update();

			// グラフィックの状態更新
			SNGraphics::Update();

			// AudioVideoの状態更新
			SNAudioVideo::Update();

			// 入力デバイスチェック
			SNInput::Update();

			// イベント更新
			SNEvent::Update();

			// アプリケーション実行
			FrameworkAppLayer.Step();

			// FPSの状態によってスキップ判定
			if (!FPSTimer.GetSkipFlag())
			{
				// 描画前処理
				FrameworkAppLayer.PreDraw();

				{
					grc = SNGraphics::GetContext();

					// 描画処理
					FrameworkAppLayer.Draw(grc);

					SNGraphics::ReleaseContext();
				}

				// サーフェスフリップ
				SNGraphics::FlipSurface();

				// 描画後処理
				FrameworkAppLayer.PostDraw();

				// FPSカウンター更新
				FPSCounter.Count();
			}
			else
			{
				FrameSkipCounter.Count();
			}

			ProcFPSCounter.Count();

			FPSCounter.Average();
			ProcFPSCounter.Average();
			FrameSkipCounter.Average();


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


// 更新
Void SNApplication::Update()
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

	// アクティブ状態を更新する
	if (EventSnapshot[SNEventActive])
	{
		Active = true;
	}

	else if (EventSnapshot[SNEventNonActive])
	{
		Active = false;
	}

	else
	{

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

