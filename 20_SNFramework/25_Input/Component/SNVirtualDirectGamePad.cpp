#include "SNVirtualDirectGamePad.h"
#include "SNGamePad.h"

Boolean* SNVirtualDirectGamePad::State[SNGamePadIDNum];
Boolean  SNVirtualDirectGamePad::Active;

// 初期化
Void SNVirtualDirectGamePad::Initialize()
{
	State[SNGamePadID1] = SNGamePad::ButtonState[SNGamePadID1];
	State[SNGamePadID2] = SNGamePad::ButtonState[SNGamePadID2];

	// デフォルト無効
	Active = false;

	return;
}

// 終了処理
Void SNVirtualDirectGamePad::Terminate()
{
	return;
}

// 状態更新
Void SNVirtualDirectGamePad::Update()
{
	// アドレスの直接参照のため更新処理不要

	return;
}

// 有効化
Void SNVirtualDirectGamePad::Activate(Boolean active)
{
	Active = active;
	return;
}
