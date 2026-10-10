#include "SNVirtualDirectGamePad.h"
#include "SNGamePad.h"

Boolean* SNVirtualDirectGamePad::State;
Boolean  SNVirtualDirectGamePad::Active;

// 初期化
Void SNVirtualDirectGamePad::Initialize()
{
	State = SNGamePad::ButtonState;

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
