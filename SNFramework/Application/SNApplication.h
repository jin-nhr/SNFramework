#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNThread.h"
#include "SNFPSTimer.h"
#include "SNFPSCounter.h"


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

	// 終了要求
	// パラメータ：終了要求 true:終了要求 false:キャンセル
	Void RequestExitApplication(Boolean request_exit);

private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNApplication();

	// スレッドクラスのユーザー実行関数
	Void UserMain();


	SNFPSTimer FPSTimer;		// FPSタイマ
	SNFPSCounter FPSCounter;	// FPSカウンター
	Boolean RequestExitFlag;	// 終了要求フラグ
};

