#pragma once
#include "../Include/SNFramework.h"
#include "SNKeyboard.h"
#include "SNMouse.h"
#include "SNGamePad.h"
#include "../Application/SNSoftTimer.h"


// ボタン判定情報
struct SNInputButtonState
{
	Boolean State;				// ボタン状態
	Boolean PreviousState;		// 前状態
	SNSoftTimer LongPressTimer;	// 長押タイマ
	SNSoftTimer RepeatTimer;	// リピートタイマ
};

// ポインティングイベント判定状態
struct SNInputPointingState
{
	Boolean Decide;			// 決定ボタン状態
	Boolean PreviousDecide;	// 決定ボタン前状態
	Boolean Cancel;			// 決定ボタン状態
	Boolean PreviousCancel;	// 決定ボタン前状態
	Boolean ClickEnable;	// クリック有効
};


// 入力装置クラス
class SNInputDevice
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

	// 更新処理
	static Void Update();

	// 入力イベント取得
	static const SNInputDeviceEvent* GetInputDeviceEvent();

	// 物理デバイス情報取得
	static const SNPhysicalInputEvent* GetPhysicalInputEvent();

private:
	// 状態更新
	static Void UpdateState();

	// デバイス入力情報更新
	static Void UpdateDevice();

	// 物理デバイス入力情報更新
	static Void UpdatePhysicalInput();

	// ボタンイベント生成
	static Void GenerateButtonEvent();

	// ポインティングイベント生成
	static Void GeneratePointingEvent();

	// アクティブ状態
	static Boolean Active;

	// 入力イベント
	static SNInputDeviceEvent InputDeviceEvent;

	// 物理入力イベント
	static SNPhysicalInputEvent PhysicalInputEvent;

	// ボタン状態
	static SNInputButtonState ButtonState[SNInputDeviceNum][SNInputButtonNum];

	// ポインティングイベント判定状態
	static SNInputPointingState PointingState;
};
