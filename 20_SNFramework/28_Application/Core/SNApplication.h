#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"
#include "SNFPSTimer.h"
#include "SNFPSCounter.h"
#include "SNStopWatch.h"
#include "SNLayerController.h"
#include "SNStateController.h"

#include "SNDebugAppOff.h"
#include "SNDebugAppRun.h"

#include "SNSysAppPreStart.h"
#include "SNSysAppStartup.h"
#include "SNSysAppNoApp.h"
#include "SNSysAppIdle.h"
#include "SNSysAppScreenKeyboard.h"
#include "SNSysAppScreenGamePad.h"
#include "SNSysAppConfig.h"
#include "SNSysAppInputConfig.h"
#include "SNSysAppPreEnd.h"

#include "SNUserAppOff.h"
#include "SNUserAppBase.h"
#include "SNBgApp.h"


// アプリケーションクラス
// アプリケーションの起動/終了、アプリケーションスレッドの制御およびメインループ実装
// アプリケーションスレッドへのイベント通知IFを持つ

class SNApplication
{
public:
	// 初期化処理
	static Void Initialize();
	
	// 起動準備
	static Void Startup();
	
	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// イベント情報取得
	static Boolean GetEventInfo(SNApplicationEvent event);

	/////////////////////////////////////
	// イベント通知
	static Void NotifyEvent(SNApplicationEvent event);

	/////////////////////////////////////
	// 情報取得
	// FPS取得
	static UInt32 GetFPS();

	// 平均時間取得
	static UInt32 GetProcTime();

	// スキップ数
	static UInt32 GetSkipFrame();

	// メイン関数
	static Void UserMain();

	// アクティブ状態
	static Boolean Active;

private:
	// イベントスナップショット
	static Void Update();

	// リザルト処理
	// リターン： true:継続、false:終了
	static Boolean EventResultProc();


	// 実行制御情報
	static SNThread* ApplicationThread;			// アプリケーションスレッド
	static SNFPSTimer FPSTimer;					// FPSタイマ
	static SNFPSCounter FPSCounter;				// FPSカウンター
	static SNFPSCounter FrameSkipCounter;		// フレームスキップカウンター
	static SNStopWatch ApplicationTimeWatcher;	// アプリケーション処理時間測定

	///////////////////////////////////////////////////////////

	// Frameworkアプリレイヤ管理
	static SNLayerController FrameworkAppLayer;

	// デバッグアプリ
	static SNStateController DebugAppStateCtrl;
	static SNDebugAppOff DebugAppOff;
	static SNDebugAppRun DebugAppRun;

	// システムアプリ
	static SNStateController SysAppStateCtrl;
	static SNSysAppPreStart SysAppPreStart;
	static SNSysAppStartup SysAppStartup;
	static SNSysAppNoApp SysAppNoApp;
	static SNSysAppIdle SysAppIdle;
	static SNSysAppScreenKeyboard SysAppScreenKeyboard;
	static SNSysAppScreenGamePad SysAppScreenGamePad;
	static SNSysAppConfig SysAppConfig;
	static SNSysAppInputConfig SysAppInputConfig;
	static SNSysAppPreEnd SysAppPreEnd;

	// ユーザーアプリ
	static SNStateController UserAppStateCtrl;
	static SNUserAppOff UserAppOff;

	// バックグラウンドアプリ
	static SNBgApp BgApp;

	///////////////////////////////////////////////////////////

	// プライマリスレッド(メインウインドウ)からのイベント通知用のためvolatileとする
	static volatile Boolean NotifyEventInfo[SNApplicationEvent::SNEventNum];	// イベント情報
	
	static Boolean EventSnapshot[SNApplicationEvent::SNEventNum];	// イベント情報スナップショット
};

