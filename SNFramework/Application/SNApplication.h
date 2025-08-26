#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNThread.h"
#include "SNFPSTimer.h"
#include "SNFPSCounter.h"
#include "SNStopWatch.h"
#include "Layer/SNLayerManager.h"
#include "Layer/SNSystemLayer.h"
#include "Layer/SNDebugLayer.h"
#include "Layer/SNApplicationLayer.h"
#include "Layer/SNBackGroundLayer.h"


// アプリケーションクラス
class SNApplication : private SNThread
{
// 共通メソッド/データ
public:
	// インスタンス生成/取得
	static SNApplication* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	// 自身のインスタンス
	static SNApplication* Me;


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNApplication();
	
	// 初期化処理
	Void Initialize();
	
	// 起動準備
	Void Startup();
	
	// 実行
	// リターン：終了コード
	Int32 Run();

	// 終了前処理
	Void BeforeTerminate();
	
	// 終了
	Void Terminate();

	// イベント情報取得
	SNApplicationEventInfo* GetEventInfo();

	/////////////////////////////////////
	// イベント通知
	// アクティブ通知
	Void NotifyActive();

	// 非アクティブ通知
	Void NotifyNonActive();

	// 終了通知
	Void NotifyExitApplication();

	// ホイールUp通知
	Void NotifyWheelUp();

	// ホイールDown通知
	Void NotifyWheelDown();

	/////////////////////////////////////
	// 情報取得
	// FPS取得
	UInt32 GetFPS();

	// 平均時間取得
	UInt32 GetProcTime();

private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNApplication();

	// スレッドクラスのユーザー実行関数
	Void UserMain();

	// イベントスナップショット
	Void SnapshotEvent();


	SNFPSTimer* FPSTimer;		// FPSタイマ
	SNFPSCounter* FPSCounter;	// FPSカウンター
	SNStopWatch* ApplicationTimeWatcher;	// アプリケーション処理時間測定

	SNLayerManager* ApplicationLayerManager;	// アプリケーションレイヤ管理

	// レイヤ
	SNDebugLayer*		DebugLayer;			// デバッグレイヤ
	SNSystemLayer*		SystemLayer;		// システムレイヤ
	SNApplicationLayer*	ApplicationLayer;	// アプリケーションレイヤ
	SNBackGroundLayer*	BackGroundLayer;	// バックグラウンドレイヤ

	SNEvent* Event;							// イベントクラス

	static const UInt8	ApplicationLayerNum = 4;		// アプリケーションレイヤ数

	SNState* ApplicationLayerList[ApplicationLayerNum];	// アプリケーションレイヤリスト
	SNApplicationEventInfo NotifyEvent;					// イベント情報
	SNApplicationEventInfo EventSnapshot;				// イベント情報スナップショット
};
