#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNThread.h"
#include "SNFPSTimer.h"
#include "SNFPSCounter.h"
#include "SNStopWatch.h"
#include "Controller/SNLayerController.h"
#include "SystemLayer/SNSystemLayer.h"
#include "DebugLayer/SNDebugLayer.h"
#include "ApplicationLayer/SNApplicationLayer.h"
#include "BackGroundLayer/SNBackGroundLayer.h"


// アプリケーションクラス
class SNApplication
{
public:
	// 初期化処理
	static Void Initialize();
	
	// 起動準備
	static Void Startup();
	
	// 実行
	// リターン：終了コード
	static Int32 Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// イベント情報取得
	static SNApplicationEventInfo* GetEventInfo();

	/////////////////////////////////////
	// イベント通知
	// アクティブ通知
	static Void NotifyActive();

	// 非アクティブ通知
	static Void NotifyNonActive();

	// 終了通知
	static Void NotifyExitApplication();

	// ホイールUp通知
	static Void NotifyWheelUp();

	// ホイールDown通知
	static Void NotifyWheelDown();

	/////////////////////////////////////
	// 情報取得
	// FPS取得
	static UInt32 GetFPS();

	// 平均時間取得
	static UInt32 GetProcTime();

	// メイン関数
	static Void UserMain();

private:
	// イベントスナップショット
	static Void SnapshotEvent();


	static SNThread* ApplicationThread;			// アプリケーションスレッド
	static SNFPSTimer FPSTimer;					// FPSタイマ
	static SNFPSCounter FPSCounter;				// FPSカウンター
	static SNStopWatch ApplicationTimeWatcher;	// アプリケーション処理時間測定
	static SNLayerController LayerController;	//レイヤ制御
	static SNDebugLayer			DebugLayer;			// デバッグレイヤ
	static SNSystemLayer		SystemLayer;		// システムレイヤ
	static SNApplicationLayer	ApplicationLayer;	// アプリケーションレイヤ
	static SNBackGroundLayer	BackGroundLayer;	// バックグラウンドレイヤ
	static SNEvent Event;							// イベントクラス
	static SNApplicationEventInfo NotifyEvent;		// イベント情報
	static SNApplicationEventInfo EventSnapshot;	// イベント情報スナップショット
};

// アプリケーションスレッド
class SNApplicationThread : public SNThread
{
private:
	// スレッドクラスのユーザー実行関数
	Void UserMain();
};
