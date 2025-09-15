#include "SNMouse.h"
#include "../System/SNWindowsAPI.h"
#include "../System/SNSystem.h"
#include "../Graphics/SNGraphics.h"
#include "../Application/SNApplication.h"

// マウスクラス

// マウス状態
SNMouseState SNMouse::MouseState = {0};

// ボタンコード配列
Int32 SNMouse::ButtonCode[SNMouseButtonNum] = {0};


// 初期化
Void SNMouse::Initialize()
{
	SNPoint mouse_pos;
	UInt32 loop_cnt;

	// 画面座標取得
	GetCursorPos((POINT*)&mouse_pos);

	// 画面座標→サーフェス座標変換
	ScreenToSurface(&mouse_pos);

	// マウス状態の初期化
	MouseState.Position = mouse_pos;
	MouseState.PosClipping = false;
	for (loop_cnt = 0; loop_cnt < SNMouseButtonNum; loop_cnt++)
	{
		MouseState.ButtonState[loop_cnt] = false;
	}

	// ボタン入れ替え状態に応じてボタンコードを設定
	if (::GetSystemMetrics(SM_SWAPBUTTON))
	{
		// 入れ替えあり
		ButtonCode[SNMouseButtonDecide] = SNKeyCodeMouseRight;
		ButtonCode[SNMouseButtonCancel] = SNKeyCodeMouseLeft;
	}
	else
	{
		// 入れ替えなし
		ButtonCode[SNMouseButtonDecide] = SNKeyCodeMouseLeft;
		ButtonCode[SNMouseButtonCancel] = SNKeyCodeMouseRight;
	}
	ButtonCode[SNMouseButtonMiddle] = SNKeyCodeMouseMiddle;
	ButtonCode[SNMouseButtonWheelUp] = SNKeyCodeNull;
	ButtonCode[SNMouseButtonWheelDown] = SNKeyCodeNull;

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
	SNPoint mouse_pos;
	UInt32 loop_cnt;

	// 画面上の座標取得
	GetCursorPos((POINT*)&mouse_pos);

	// 画面座標→サーフェス座標変換
	MouseState.PosClipping = ScreenToSurface(&mouse_pos);

	// マウス状態の位置を更新
	MouseState.Position = mouse_pos;

	// ボタンコードに応じて入力状態をチェック
	for (loop_cnt = 0; loop_cnt < SNMouseClickNum; loop_cnt++)
	{
		MouseState.ButtonState[loop_cnt] = (Boolean)((GetAsyncKeyState(ButtonCode[loop_cnt]) & 0x8000) != 0);
	}

	// ホイール状態をApplicationから取得
	MouseState.ButtonState[SNMouseButtonWheelUp] = SNApplication::GetEventInfo()->WheelUp;
	MouseState.ButtonState[SNMouseButtonWheelDown] = SNApplication::GetEventInfo()->WheelDown;

	return;
}

// 画面座標系→サーフェス座標変換
// リターン：座標クリッピング有無
Boolean SNMouse::ScreenToSurface(SNPoint* point)
{
	// 画面座標からクライアント座標に変換
	SNSystem::ScreenToClient(point);

	// クライアン座標ト系→サーフェス座標に変換
	return SNGraphics::ClientToSurface(point);
}
