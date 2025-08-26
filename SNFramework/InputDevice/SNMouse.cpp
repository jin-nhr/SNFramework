#include "SNMouse.h"
#include "../System/SNWindowsAPI.h"
#include "../System/SNSystem.h"
#include "../Graphics/SNGraphics.h"
#include "../Application/SNApplication.h"

// マウスクラス

// コンストラクタ
SNMouse::SNMouse()
{
	UInt32 loop_cnt;

	// 変数初期化
	MouseState = {0};

	for (loop_cnt = 0; loop_cnt < SNMouseButtonNum; loop_cnt++)
	{
		ButtonCode[loop_cnt] = 0;
	}
	return;
}

// デストラクタ
SNMouse::~SNMouse()
{
	return;
}

// 初期化
Void SNMouse::Initialize()
{
	POINT winapi_point;
	UInt32 loop_cnt;

	GetCursorPos(&winapi_point);

	// マウス状態の初期化
	MouseState.Position.X = winapi_point.x;
	MouseState.Position.Y = winapi_point.y;
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
	ButtonCode[SNMouseButtonX1] = SNKeyCodeMouseX1;
	ButtonCode[SNMouseButtonX2] = SNKeyCodeMouseX2;
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
	POINT winapi_point;
	SNPoint mouse_pos;
	UInt32 loop_cnt;

	// 画面上の座標取得
	GetCursorPos(&winapi_point);

	mouse_pos.X = winapi_point.x;
	mouse_pos.Y = winapi_point.y;

	// 画面座標からクライアント座標に変換
	SNSystem::GetInstance()->ScreenToClient(&mouse_pos);

	//画面座標系→サーフェス座標に変換
	SNGraphics::GetInstance()->ScreenToSurface(&mouse_pos);

	// マウス状態の位置を更新
	MouseState.Position = mouse_pos;

	// ボタンコードに応じて入力状態をチェック
	for (loop_cnt = 0; loop_cnt < SNMouseButtonNum; loop_cnt++)
	{
		MouseState.ButtonState[loop_cnt] = (Boolean)((GetAsyncKeyState(ButtonCode[loop_cnt]) & 0x8000) != 0);
	}

	// ホイール状態をApplicationから取得
	MouseState.ButtonState[SNMouseButtonWheelUp] = SNApplication::GetInstance()->GetEventInfo()->WheelUp;
	MouseState.ButtonState[SNMouseButtonWheelDown] = SNApplication::GetInstance()->GetEventInfo()->WheelDown;

	return;
}

// マウス状態取得
const SNMouseState* SNMouse::GetState()
{
	return &MouseState;
}
