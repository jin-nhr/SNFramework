#pragma once
#include "SNFrameworkInternal.h"

// ポインティング状態
enum SNVirtualPointingState
{
	SNVirtualPointingStateTop,
	SNVirtualPointingStateDecide = SNVirtualPointingStateTop,	// 決定ボタン状態
	SNVirtualPointingStatePreviousDecide,	// 決定ボタン前状態
	SNVirtualPointingStateCancel,			// キャンセルボタン状態
	SNVirtualPointingStatePreviousCancel,	// キャンセルボタン前状態
	SNVirtualPointingStateExBtn,				// 拡張ボタン状態
	SNVirtualPointingStatePreviousExBtn,		// 拡張ボタン前状態
	SNVirtualPointingStateClickEnable,		// クリック有効
	SNVirtualPointingStatePreviousDrag,		// 前回ドラッグ中
	SNVirtualPointingStatePreviousFlick,	// 前回フリック中
	SNVirtualPointingStateNum
};



// 仮想ポインティングデバイス
// 実デバイス(Mouse)の入力情報から仮想デバイスの入力情報に変換しアプリケーションに提供する

class SNVirtualPointingDevice
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 更新処理
	static Void Update();

	static Boolean Event[SNVirtualPointingEventNum];	// ポインティングイベント
	static SNPoint Info[SNVirtualPointingInfoNum];		// ポインティング情報

	// イベント有無
	static Boolean EventExist;

private:

	// 更新前処理
	static Void UpdatePrevProc();

	// 更新処理(決定)
	static Void UpdateDecide();

	// 更新処理(キャンセル)
	static Void UpdateCancel();

	// 更新処理(ホイール)
	static Void UpdateWheel();

	// 更新処理(移動)
	static Void UpdateMove();

	// 更新処理(ドラッグ)
	static Void UpdateDrag();

	// 更新処理(ドロップ)
	static Void UpdateDrop();

	// 更新処理(フリック)
	static Void UpdateFlick();

	// 更新処理(拡張ボタン)
	static Void UpdateExBtn();

	static Boolean State[SNVirtualPointingStateNum];		// ポインティング状態

};
