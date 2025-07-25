#include "SNApplication.h"
#include "../System/SNWindowsAPI.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNSystem.h"
#include "../Graphics/SNGraphics.h"


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

	return;
}

// 起動準備
Void SNApplication::Startup()
{
	// コンフィギュレーション取得
	SNConfigurationData* configuration = &SNConfiguration::GetInstance()->ConfigurationData;

	// FPS設定
	FPSTimer.SetPFS(configuration->System.FPS);

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


	return;
}

// 終了
Void SNApplication::Terminate()
{


	return;
}

// 終了要求
// パラメータ：終了要求 true:終了要求 false:キャンセル
Void SNApplication::RequestExitApplication(Boolean request_exit)
{
	// スレッド動作中
	if (GetRunStatus())
	{
		// 動作中は要求に応じて停止フラグを設定する
		RequestExitFlag = true;
	}

	// スレッド非動作中
	else
	{
		// 終了通知を送る
		SNSystem::GetInstance()->NoticeExitApplication(true);
	}

	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNApplication::SNApplication() : SNThread()
{
	// 変数初期化
	RequestExitFlag = false;

	return;
}


// スレッドクラスのユーザー実行関数
Void SNApplication::UserMain()
{
	SNGraphics* graphics = SNGraphics::GetInstance();

	// FPSタイマを起動
	FPSTimer.Start();

	// FPSカウンター起動
	FPSCounter.Start();

	// メインループ
	while (true)
	{
		// フレーム処理タイミングか確認
		if (FPSTimer.CheckTimeout())
		{
			// FPSタイマリスタート
			FPSTimer.Restart();


			////////////////////////
			// 暫定終了処理

			if (RequestExitFlag)
			{
				// ループを抜ける
				break;
			}


			////////////////////////


			// 入力デバイスチェック

			// その他イベントチェック

			// アプリケーション実行

			// 描画処理
			BitBlt(
				(HDC)SNGraphics::GetInstance()->GetSurface()->GetDC()->GetDeviceContext(),
				0,
				0,
				SNGraphics::GetInstance()->GetSurface()->GetWidth(),
				SNGraphics::GetInstance()->GetSurface()->GetHeight(),
				0,
				0,
				0,
				WHITENESS);

			// サーフェスフリップ
			graphics->FlipSurface();

			// 画面更新通知
			SNSystem::GetInstance()->NoticeRefreshScreen();

			// FPSカウンター更新
			FPSCounter.Count();
		}

		else
		{
			// Sleepする
			FPSTimer.Sleep();
		}
	}

	// 終了通知を送る
	SNSystem::GetInstance()->NoticeExitApplication(true);

	return;
}

