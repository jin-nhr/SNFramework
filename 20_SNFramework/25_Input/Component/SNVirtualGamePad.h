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

	// ボタンイベント
	static Boolean Event[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum][SNVirtualGamePadEventNum];

	// イベント有無
	static Boolean EventExist[SNVirtualGamePadIDNum];

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

	// ボタンイベント更新(パッドID指定)
	static Void UpdateEventPadID(SNVirtualGamePadID pad_id);

	// ボタン状態
	static SNVirtualGamePadButtonState ButtonState[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum];
};
