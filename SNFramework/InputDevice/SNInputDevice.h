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
};


// 入力装置クラス
class SNInputDevice
{
// 共通メソッド/データ
public:
	// インスタンス生成/取得
	static SNInputDevice* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	// 自身のインスタンス
	static SNInputDevice* Me;


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNInputDevice();
	
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

	// 更新処理
	Void Update();

	// 入力イベント取得
	const SNInputDeviceEvent* GetInputDeviceEvent();

	// 物理デバイス情報取得
	const SNPhysicalInputEvent* GetPhysicalInputEvent();

private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNInputDevice();

	// 状態更新
	Void UpdateState();

	// デバイス入力情報更新
	Void UpdateDevice();

	// 物理デバイス入力情報更新
	Void UpdatePhysicalInput();

	// ボタンイベント生成
	Void GenerateButtonEvent();

	// ポインティングイベント生成
	Void GeneratePointingEvent();

	// アクティブ状態
	Boolean Active;

	// キーボード
	SNKeyboard*	Keyboard;

	// マウス
	SNMouse*	Mouse;

	// ゲームパッド
	SNGamePad*	GamePad[SNGamePadIDNum];

	// 入力イベント
	SNInputDeviceEvent InputDeviceEvent;

	// 物理入力イベント
	SNPhysicalInputEvent PhysicalInputEvent;

	// ボタン状態
	SNInputButtonState ButtonState[SNInputDeviceNum][SNInputButtonNum];

	// ポインティングイベント判定状態
	SNInputPointingState PointingState;


	// 非アクティブキー状態
	Boolean NonActiveKeyState[SNKeyCodeNum];

	// 非アクティブマウス状態
	Boolean NonActiveMouseButtonState[SNMouseButtonNum];

	// 非アクティブポインティング座標
	SNPoint NonActivePointingPosition;

	// 非アクティブボタン状態
	Boolean NonActiveGamePadButtonState[SNGamePadButtonNum];
};
