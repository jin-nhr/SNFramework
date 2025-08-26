#include "SNGamePad.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNWindowsAPI.h"


// ゲームパッドクラス

// コンストラクタ
SNGamePad::SNGamePad()
{
	UInt32 loop_cnt;
	SNAxisInfo* axis_info;

	// 変数初期化
	GamePadInfo.ID = 0;
	GamePadInfo.Enable = false;
	GamePadInfo.ButtonNum = 0;

	for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
	{
		axis_info = &GamePadInfo.AxisInfo[loop_cnt];

		axis_info->Enable = false;
		axis_info->High = 0;
		axis_info->High_Threshold = 0;
		axis_info->Low = 0;
		axis_info->Low_Threshold = 0;
		axis_info->Center = 0;
	}

	for (loop_cnt = 0; loop_cnt < SNGamePadButtonNum; loop_cnt++)
	{
		ButtonState[loop_cnt] = false;
	}

	return;
}

// デストラクタ
SNGamePad::~SNGamePad()
{
	return;
}

// 初期化
Void SNGamePad::Initialize(UInt8 id)
{
	Int loop_cnt;

	// デバイス情報更新
	GamePadInfoUpdate(id);

	// ボタン状態初期化
	for (loop_cnt = 0; loop_cnt < SNGamePadButtonNum; loop_cnt++)
	{
		ButtonState[loop_cnt] = false;
	}

	return;
}

// 終了
void SNGamePad::Terminate()
{
	return;
}

// 更新
void SNGamePad::Update()
{
	JOYINFOEX joyinfo;
	Boolean enable;
	UInt32 loop_cnt;
	UInt32 axis_value[SNGamePadAxisNum];

	// ゲームパッド状態取得
	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL;

	// joyGetPosのリターンでパッドの有効/無効を判断
	if (joyGetPosEx(GamePadInfo.ID, &joyinfo) == JOYERR_NOERROR)
	{
		enable = true;
	}
	else
	{
		enable = false;
	}

	// ゲームパッド有効
	if (enable)
	{
		// 無効→有効に切り替わった
		if (!GamePadInfo.Enable)
		{
			// 情報更新
			GamePadInfoUpdate(GamePadInfo.ID);
		}

		// ボタン入力状態の更新
		// ボタンの押下状態はビット割り当てなので下位から順に1ビットずつチェック
		for (loop_cnt = 0; loop_cnt < GamePadInfo.ButtonNum; loop_cnt++)
		{
			if ((joyinfo.dwButtons & (0x00000001 << loop_cnt)) != 0)
			{
				ButtonState[SNGamePadButtonTop + loop_cnt] = true;
			}
			else
			{
				ButtonState[SNGamePadButtonTop + loop_cnt] = false;
			}
		}

		// 軸情報の更新
		// 6軸のデータを配列にセット
		axis_value[SNGamePadAxisX] = joyinfo.dwXpos;
		axis_value[SNGamePadAxisY] = joyinfo.dwYpos;
		axis_value[SNGamePadAxisZ] = joyinfo.dwZpos;
		axis_value[SNGamePadAxisR] = joyinfo.dwRpos;
		axis_value[SNGamePadAxisU] = joyinfo.dwUpos;
		axis_value[SNGamePadAxisV] = joyinfo.dwVpos;

		// 6軸分の上下チェック
		for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
		{
			// Low側
			if (axis_value[loop_cnt] < GamePadInfo.AxisInfo[loop_cnt].Low_Threshold)
			{
				ButtonState[SNGamePadAxisTop + (loop_cnt * 2)] = true;
			}
			else
			{
				ButtonState[SNGamePadAxisTop + (loop_cnt * 2)] = false;
			}

			// High側
			if (axis_value[loop_cnt] > GamePadInfo.AxisInfo[loop_cnt].High_Threshold)
			{
				ButtonState[SNGamePadAxisTop + (loop_cnt * 2) + 1] = true;
			}
			else
			{
				ButtonState[SNGamePadAxisTop + (loop_cnt * 2) + 1] = false;
			}
		}


		// POV入力確認
		// POVは上を0度としたミリ度の値を入ってくる
		// 360度を12分割し4ブロック(120度)を1方向に割り当て、上下左右を判定する

		// POV 0 ～ 35900
		if (joyinfo.dwPOV <= 35900)
		{
			// 300度 - 60度
			ButtonState[SNGamePadPOVUp] = ((30000 <= joyinfo.dwPOV) || (joyinfo.dwPOV <= 6000));


			// 30度 - 150度
			ButtonState[SNGamePadPOVRight] = ((3000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 15000));


			// 120度 - 240度
			ButtonState[SNGamePadPOVDown] = ((12000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 24000));


			// 210度 - 330度
			ButtonState[SNGamePadPOVLeft] = ((21000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 33000));

		}
		// 入力なし
		else
		{
			ButtonState[SNGamePadPOVUp] = false;
			ButtonState[SNGamePadPOVRight] = false;
			ButtonState[SNGamePadPOVDown] = false;
			ButtonState[SNGamePadPOVLeft] = false;
		}
	}

	// ゲームパッド無効
	else
	{
		// 有効→無効に切り替わった
		if (GamePadInfo.Enable)
		{
			// 情報更新
			GamePadInfoUpdate(GamePadInfo.ID);

			// ジョイパッド無効
			for (loop_cnt = 0; loop_cnt < SNGamePadButtonNum; loop_cnt++)
			{
				ButtonState[loop_cnt] = false;
			}
		}
	}

	return;
}

// ゲームパッド情報更新
void SNGamePad::GamePadInfoUpdate(UInt8 id)
{
	JOYINFOEX joyinfo;
	JOYCAPS joycaps;
	SNAxisInfo* axis_info;
	UInt32 loop_cnt;

	// ID保存
	GamePadInfo.ID = id;

	// JOUINFOの設定
	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL;

	// パッド情報取得
	if ((joyGetPosEx(GamePadInfo.ID, &joyinfo) == JOYERR_NOERROR))
	{
		GamePadInfo.Enable = true;
	}
	else
	{
		GamePadInfo.Enable = false;
	}

	// パッド有効時のみ処理
	if (GamePadInfo.Enable)
	{
		// パッド性能取得
		joyGetDevCaps(GamePadInfo.ID, &joycaps, sizeof(joycaps));

		// ボタン数設定
		GamePadInfo.ButtonNum = joycaps.wNumButtons;

		// 軸情報更新
		// X軸
		GamePadInfo.AxisInfo[SNGamePadAxisX].Enable = (joycaps.wNumAxes >= 2);
		GamePadInfo.AxisInfo[SNGamePadAxisX].High = joycaps.wXmax;
		GamePadInfo.AxisInfo[SNGamePadAxisX].Low = joycaps.wXmin;

		// Y軸
		GamePadInfo.AxisInfo[SNGamePadAxisY].Enable = (joycaps.wNumAxes >= 2);
		GamePadInfo.AxisInfo[SNGamePadAxisY].High = joycaps.wYmax;
		GamePadInfo.AxisInfo[SNGamePadAxisY].Low = joycaps.wYmin;

		// Z軸
		GamePadInfo.AxisInfo[SNGamePadAxisZ].Enable = (joycaps.wCaps & JOYCAPS_HASZ);
		GamePadInfo.AxisInfo[SNGamePadAxisZ].High = joycaps.wZmax;
		GamePadInfo.AxisInfo[SNGamePadAxisZ].Low = joycaps.wZmin;

		// R軸
		GamePadInfo.AxisInfo[SNGamePadAxisR].Enable = (joycaps.wCaps & JOYCAPS_HASR);
		GamePadInfo.AxisInfo[SNGamePadAxisR].High = joycaps.wRmax;
		GamePadInfo.AxisInfo[SNGamePadAxisR].Low = joycaps.wRmin;

		// U軸
		GamePadInfo.AxisInfo[SNGamePadAxisU].Enable = (joycaps.wCaps & JOYCAPS_HASU);
		GamePadInfo.AxisInfo[SNGamePadAxisU].High = joycaps.wUmax;
		GamePadInfo.AxisInfo[SNGamePadAxisU].Low = joycaps.wUmin;

		// V軸
		GamePadInfo.AxisInfo[SNGamePadAxisV].Enable = (joycaps.wCaps & JOYCAPS_HASV);
		GamePadInfo.AxisInfo[SNGamePadAxisV].High = joycaps.wVmax;
		GamePadInfo.AxisInfo[SNGamePadAxisV].Low = joycaps.wVmin;

		// 中央値、閾値計算
		for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
		{
			axis_info = &GamePadInfo.AxisInfo[loop_cnt];

			// 中央値
			axis_info->Center = (axis_info->High + axis_info->Low) / 2;

			// 閾値
			// CenterからLow/Highの中間値を閾値とする
			axis_info->High_Threshold = axis_info->High - ((axis_info->High - axis_info->Center) / 2);
			axis_info->Low_Threshold = axis_info->Low + ((axis_info->Center - axis_info->Low) / 2);
		}
	}

	// ゲームパッド無効時
	else
	{
		// ボタン数0
		GamePadInfo.ButtonNum = 0;

		// j軸情報初期化
		for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
		{
			axis_info = &GamePadInfo.AxisInfo[loop_cnt];

			axis_info->Enable = false;
			axis_info->High = 0;
			axis_info->High_Threshold = 0;
			axis_info->Low = 0;
			axis_info->Low_Threshold = 0;
			axis_info->Center = 0;
		}
	}

	return;
}

// 状態取得
const Boolean* SNGamePad::GetState()
{
	return ButtonState;
}
