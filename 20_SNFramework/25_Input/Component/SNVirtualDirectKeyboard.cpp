#include "SNVirtualDirectKeyboard.h"
#include "SNKeyboard.h"

// 入力状態
Boolean* SNVirtualDirectKeyboard::State = nullptr;
Boolean  SNVirtualDirectKeyboard::Active;

// 初期化
Void SNVirtualDirectKeyboard::Initialize()
{
	State = SNKeyboard::KeyState;

	// デフォルト無効
	Active = false;

	return;
}

// 終了処理
Void SNVirtualDirectKeyboard::Terminate()
{
	return;
}

// 状態更新
Void SNVirtualDirectKeyboard::Update()
{
	// アドレスの直接参照のため更新処理不要

	return;
}

// 有効化
Void SNVirtualDirectKeyboard::Activate(Boolean active)
{
	Active = active;
	return;
}
