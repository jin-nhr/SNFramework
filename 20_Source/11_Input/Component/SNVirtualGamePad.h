#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoftTimer.h"

// ボタン状態
struct SNVirtualGamePadButtonState
{
	Boolean State;				// ボタン状態
	Boolean PreviousState;		// 前状態
	SNSoftTimer LongPressTimer;	// 長押タイマ
	SNSoftTimer RepeatTimer;		// リピートタイマ
};

// 仮想ゲームパッドクラス
// 実デバイス(Keyboard, GamePad)の入力情報から仮想デバイスの入力情報に変換しアプリケーションに提供する
class SNVirtualGamePad
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 更新処理
	static Void Update();

	// Dir上
	static Boolean DPadUpPush();
	static Boolean DPadUpPress();
	static Boolean DPadUpRepeat();
	static Boolean DPadUpLong();
	static Boolean DPadUpRelease();

	// Dir下
	static Boolean DPadDownPush();
	static Boolean DPadDownPress();
	static Boolean DPadDownRepeat();
	static Boolean DPadDownLong();
	static Boolean DPadDownRelease();

	// Dir左
	static Boolean DPadLeftPush();
	static Boolean DPadLeftPress();
	static Boolean DPadLeftRepeat();
	static Boolean DPadLeftLong();
	static Boolean DPadLeftRelease();

	// Dir右
	static Boolean DPadRightPush();
	static Boolean DPadRightPress();
	static Boolean DPadRightRepeat();
	static Boolean DPadRightLong();
	static Boolean DPadRightRelease();

	// A
	static Boolean APush();
	static Boolean APress();
	static Boolean ARepeat();
	static Boolean APadALong();
	static Boolean APadARelease();

	// B
	static Boolean BPush();
	static Boolean BPress();
	static Boolean BRepeat();
	static Boolean BLong();
	static Boolean BRelease();

	// X
	static Boolean XPush();
	static Boolean XPress();
	static Boolean XRepeat();
	static Boolean XLong();
	static Boolean XRelease();

	// Y
	static Boolean YPush();
	static Boolean YPress();
	static Boolean YRepeat();
	static Boolean YLong();
	static Boolean YRelease();

	// Select
	static Boolean SelectPush();
	static Boolean SelectPress();
	static Boolean SelectRepeat();
	static Boolean SelectLong();
	static Boolean SelectRelease();

	// Start
	static Boolean StartPush();
	static Boolean StartPress();
	static Boolean StartRepeat();
	static Boolean StartLong();
	static Boolean StartRelease();

	// L1
	static Boolean L1Push();
	static Boolean L1Press();
	static Boolean L1Repeat();
	static Boolean L1Long();
	static Boolean L1Release();

	// R1
	static Boolean R1Push();
	static Boolean R1Press();
	static Boolean R1Repeat();
	static Boolean R1Long();
	static Boolean R1Release();

	// L2
	static Boolean L2Push();
	static Boolean L2Press();
	static Boolean L2Repeat();
	static Boolean L2Long();
	static Boolean L2Release();

	// R2
	static Boolean R2Push();
	static Boolean R2Press();
	static Boolean R2Repeat();
	static Boolean R2Long();
	static Boolean R2Release();

	// L3
	static Boolean L3Push();
	static Boolean L3Press();
	static Boolean L3Repeat();
	static Boolean L3Long();
	static Boolean L3Release();

	// R3
	static Boolean R3Push();
	static Boolean R3Press();
	static Boolean R3Repeat();
	static Boolean R3Long();
	static Boolean R3Release();

	// L Stick Up
	static Boolean LStkUpPush();
	static Boolean LStkUpPress();
	static Boolean LStkUpRepeat();
	static Boolean LStkUpLong();
	static Boolean LStkUpRelease();

	// L Stick Down
	static Boolean LStkDownPush();
	static Boolean LStkDownPress();
	static Boolean LStkDownRepeat();
	static Boolean LStkDownLong();
	static Boolean LStkDownRelease();

	// L Stick left
	static Boolean LStkLeftPush();
	static Boolean LStkLeftPress();
	static Boolean LStkLeftRepeat();
	static Boolean LStkLeftLong();
	static Boolean LStkLeftRelease();

	// L Stick Right
	static Boolean LStkRightPush();
	static Boolean LStkRightPress();
	static Boolean LStkRightRepeat();
	static Boolean LStkRightLong();
	static Boolean LStkRightRelease();

	// R Stick Up
	static Boolean RStkUpPush();
	static Boolean RStkUpPress();
	static Boolean RStkUpRepeat();
	static Boolean RStkUpLong();
	static Boolean RStkUpRelease();

	// R Stick Down
	static Boolean RStkDownPush();
	static Boolean RStkDownPress();
	static Boolean RStkDownRepeat();
	static Boolean RStkDownLong();
	static Boolean RStkDownRelease();

	// R Stick left
	static Boolean RStkLeftPush();
	static Boolean RStkLeftPress();
	static Boolean RStkLeftRepeat();
	static Boolean RStkLeftLong();
	static Boolean RStkLeftRelease();

	// R Stick Right
	static Boolean RStkRightPush();
	static Boolean RStkRightPress();
	static Boolean RStkRightRepeat();
	static Boolean RStkRightLong();
	static Boolean RStkRightRelease();

	// イベント有無
	static Boolean EventExist;

private:

	// ボタン状態更新
	// 入力デバイスの情報を統合しボタン状態を更新する
	static Void UpdateButtonState();

	// 前状態更新
	// ボタン状態更新前準備
	static Void UpdatePrevState();

	// ゲームパッド反映
	// ゲームパッド入力状態をボタン状態に反映
	static Void UpdateButtonStateFromGamePad();

	// キーボード反映
	// キーボード入力状態をボタン状態に反映
	static Void UpdateButtonStateKeyboard();

	// ボタンイベント更新
	// ボタン状態をもとにボタンイベントを更新する
	static Void UpdateButtonEvent();

	// ボタンイベント
	static Boolean Event[SNVirtualGamePadButtonNum][SNVirtualGamePadEventNum];

	// ボタン状態
	static SNVirtualGamePadButtonState ButtonState[SNVirtualGamePadButtonNum];
};
