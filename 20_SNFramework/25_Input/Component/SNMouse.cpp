#include "SNMouse.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNGraphics.h"
#include "SNApplication.h"
#include "SNInput.h"

// マウスクラス

SNPoint SNMouse::Position;							// サーフェス上の論理座標
Boolean SNMouse::PosClipping;						// 座標クリップした？(マウスが画面外か？)
Boolean SNMouse::ButtonState[SNMouseButtonNum];		// マウスボタン状態
Boolean SNMouse::ButtonSwap;							// 左右ボタン入替


// 初期化
Void SNMouse::Initialize()
{
	UInt32 loop_cnt;

	// 初期値設定
	UpdatePos();

	// ボタン状態初期化
	for (loop_cnt = 0; loop_cnt < SNMouseButtonNum; loop_cnt++)
	{
		ButtonState[loop_cnt] = false;
	}

	// ボタン入れ替え状態に応じてボタンコードを設定
	ButtonSwap = (::GetSystemMetrics(SM_SWAPBUTTON) != 0);

	return;
}

// 終了処理
Void SNMouse::Terminate()
{
	return;
}

// 状態更新
Void SNMouse::Update()
{
	UpdatePos();
	UpdateButtonState();

	return;
}


// 座標更新
Void SNMouse::UpdatePos()
{
	SNPoint pnt;

	// 画面上の座標取得
	// SNPointとPOINTは互換性ありのためそのまま渡す
	GetCursorPos((POINT*)&pnt);

	// 画面座標からクライアント座標に変換
	SNSystem::ScreenToClient(&pnt);

	// クライアン座標ト系→サーフェス座標に変換
	PosClipping = SNGraphics::ClientToSurface(&pnt);

	// 座標更新
	Position = pnt;

	return;
}

// ボタン状態更新
Void SNMouse::UpdateButtonState()
{
	SNMouseButton code_left;
	SNMouseButton code_right;

	// 左右入れ替え確認
	if (ButtonSwap)
	{
		code_left = SNMouseButtonCancel;
		code_right = SNMouseButtonDecide;
	}
	else
	{
		// こっちが入替なし
		code_left = SNMouseButtonDecide;
		code_right = SNMouseButtonCancel;
	}

	// マウスボタン状態
	ButtonState[code_left] = (Boolean)((GetAsyncKeyState(SNKeyCodeMouseLeft) & 0x8000) != 0);
	ButtonState[code_right] = (Boolean)((GetAsyncKeyState(SNKeyCodeMouseRight) & 0x8000) != 0);
	ButtonState[SNMouseButtonMiddle] = (Boolean)((GetAsyncKeyState(SNKeyCodeMouseMiddle) & 0x8000) != 0);

	// ホイール状態をApplicationから取得
	ButtonState[SNMouseButtonWheelUp] = SNApplication::GetEventInfo(SNEventWheelUp);
	ButtonState[SNMouseButtonWheelDown] = SNApplication::GetEventInfo(SNEventWheelDown);

	return;
}
