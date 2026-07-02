#include "SNGamePad.h"
#include "SNConfig.h"
#include "SNWindowsAPI.h"
#include "SNInput.h"

// ゲームパッドクラス

// ゲームパッド情報
SNGamePadInfo SNGamePad::GamePadInfo[SNGamePadIDNum] = { 0 };

// ボタン状態
Boolean SNGamePad::ButtonState[SNGamePadIDNum][SNGamePadButtonNum] = { 0 };

// 初期化
Void SNGamePad::Initialize()
{
	OnInitialize(SNGamePadID1);
	OnInitialize(SNGamePadID2);
	return;
}

// 終了処理
Void SNGamePad::Terminate()
{
	OnTerminate(SNGamePadID1);
	OnTerminate(SNGamePadID2);
	return;
}

// 更新
Void SNGamePad::Update()
{
	OnUpdate(SNGamePadID1);
	OnUpdate(SNGamePadID2);

	return;
}

// 初期化
Void SNGamePad::OnInitialize(UInt8 id)
{
	Int loop_cnt;

	// デバイス情報更新
	GamePadInfoUpdate(id);

	// ボタン状態初期化
	for (loop_cnt = 0; loop_cnt < SNGamePadButtonNum; loop_cnt++)
	{
		ButtonState[id][loop_cnt] = false;
	}

	return;
}

// 終了
void SNGamePad::OnTerminate(UInt8 id)
{
	return;
}

// 更新
void SNGamePad::OnUpdate(UInt8 id)
{
	JOYINFOEX joyinfo;
	Boolean enable;
	UInt32 loop_cnt;
	UInt32 axis_value[SNGamePadAxisNum];

	// ゲームパッド状態取得
	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL;

	// joyGetPosのリターンでパッドの有効/無効を判断
	if (joyGetPosEx(GamePadInfo[id].ID, &joyinfo) == JOYERR_NOERROR)
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
		if (!GamePadInfo[id].Enable)
		{
			// 情報更新
			GamePadInfoUpdate(GamePadInfo[id].ID);
		}

		////////////////////////////////////////////////////////////////////////////////
		// ボタン入力状態の更新
		// ボタンの押下状態はビット割り当てなので下位から順に1ビットずつチェック
		for (loop_cnt = 0; loop_cnt < GamePadInfo[id].ButtonNum; loop_cnt++)
		{
			if ((joyinfo.dwButtons & (0x00000001 << loop_cnt)) != 0)
			{
				ButtonState[id][SNGamePadButtonTop + loop_cnt] = true;
			}
			else
			{
				ButtonState[id][SNGamePadButtonTop + loop_cnt] = false;
			}
		}

		////////////////////////////////////////////////////////////////////////////////
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
			if (axis_value[loop_cnt] < GamePadInfo[id].AxisInfo[loop_cnt].Low_Threshold)
			{
				ButtonState[id][SNGamePadAxisTop + (loop_cnt * 2)] = true;
			}
			else
			{
				ButtonState[id][SNGamePadAxisTop + (loop_cnt * 2)] = false;
			}

			// High側
			if (axis_value[loop_cnt] > GamePadInfo[id].AxisInfo[loop_cnt].High_Threshold)
			{
				ButtonState[id][SNGamePadAxisTop + (loop_cnt * 2) + 1] = true;
			}
			else
			{
				ButtonState[id][SNGamePadAxisTop + (loop_cnt * 2) + 1] = false;
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		// POV入力確認
		// POVは上を0度としたミリ度の値を入ってくる
		// 360度を12分割し4ブロック(120度)を1方向に割り当て、上下左右を判定する

		// POV 0 ～ 35900
		if (joyinfo.dwPOV <= 35900)
		{
			// 300度 - 60度
			ButtonState[id][SNGamePadPOVUp] = ((30000 <= joyinfo.dwPOV) || (joyinfo.dwPOV <= 6000));


			// 30度 - 150度
			ButtonState[id][SNGamePadPOVRight] = ((3000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 15000));


			// 120度 - 240度
			ButtonState[id][SNGamePadPOVDown] = ((12000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 24000));


			// 210度 - 330度
			ButtonState[id][SNGamePadPOVLeft] = ((21000 <= joyinfo.dwPOV) && (joyinfo.dwPOV <= 33000));

		}
		// 入力なし
		else
		{
			ButtonState[id][SNGamePadPOVUp] = false;
			ButtonState[id][SNGamePadPOVRight] = false;
			ButtonState[id][SNGamePadPOVDown] = false;
			ButtonState[id][SNGamePadPOVLeft] = false;
		}
	}

	// ゲームパッド無効
	else
	{
		// 有効→無効に切り替わった
		if (GamePadInfo[id].Enable)
		{
			// 情報更新
			GamePadInfoUpdate(GamePadInfo[id].ID);

			// ジョイパッド無効
			for (loop_cnt = 0; loop_cnt < SNGamePadButtonNum; loop_cnt++)
			{
				ButtonState[id][loop_cnt] = false;
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
	GamePadInfo[id].ID = id;

	// JOUINFOの設定
	joyinfo.dwSize = sizeof(joyinfo);
	joyinfo.dwFlags = JOY_RETURNALL;

	// パッド情報取得
	if ((joyGetPosEx(GamePadInfo[id].ID, &joyinfo) == JOYERR_NOERROR))
	{
		GamePadInfo[id].Enable = true;
	}
	else
	{
		GamePadInfo[id].Enable = false;
	}

	// パッド有効時のみ処理
	if (GamePadInfo[id].Enable)
	{
		// パッド性能取得
		joyGetDevCaps(GamePadInfo[id].ID, &joycaps, sizeof(joycaps));

		// ボタン数設定
		GamePadInfo[id].ButtonNum = joycaps.wNumButtons;

		// 軸情報更新
		// X軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisX].Enable = (joycaps.wNumAxes >= 2);
		GamePadInfo[id].AxisInfo[SNGamePadAxisX].High = joycaps.wXmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisX].Low = joycaps.wXmin;

		// Y軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisY].Enable = (joycaps.wNumAxes >= 2);
		GamePadInfo[id].AxisInfo[SNGamePadAxisY].High = joycaps.wYmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisY].Low = joycaps.wYmin;

		// Z軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisZ].Enable = (joycaps.wCaps & JOYCAPS_HASZ);
		GamePadInfo[id].AxisInfo[SNGamePadAxisZ].High = joycaps.wZmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisZ].Low = joycaps.wZmin;

		// R軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisR].Enable = (joycaps.wCaps & JOYCAPS_HASR);
		GamePadInfo[id].AxisInfo[SNGamePadAxisR].High = joycaps.wRmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisR].Low = joycaps.wRmin;

		// U軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisU].Enable = (joycaps.wCaps & JOYCAPS_HASU);
		GamePadInfo[id].AxisInfo[SNGamePadAxisU].High = joycaps.wUmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisU].Low = joycaps.wUmin;

		// V軸
		GamePadInfo[id].AxisInfo[SNGamePadAxisV].Enable = (joycaps.wCaps & JOYCAPS_HASV);
		GamePadInfo[id].AxisInfo[SNGamePadAxisV].High = joycaps.wVmax;
		GamePadInfo[id].AxisInfo[SNGamePadAxisV].Low = joycaps.wVmin;

		// 中央値、閾値計算
		for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
		{
			axis_info = &GamePadInfo[id].AxisInfo[loop_cnt];

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
		GamePadInfo[id].ButtonNum = 0;

		// j軸情報初期化
		for (loop_cnt = 0; loop_cnt < SNGamePadAxisNum; loop_cnt++)
		{
			axis_info = &GamePadInfo[id].AxisInfo[loop_cnt];

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

