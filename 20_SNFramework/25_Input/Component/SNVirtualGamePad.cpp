#include "SNVirtualGamePad.h"
#include "SNConfig.h"
#include "SNInput.h"
#include "SNGamePad.h"
#include "SNKeyboard.h"

Boolean SNVirtualGamePad::Event[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum][SNVirtualGamePadEventNum];
SNVirtualGamePadButtonState SNVirtualGamePad::ButtonState[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum];
Boolean SNVirtualGamePad::EventExist[SNVirtualGamePadIDNum];


// 初期化
Void SNVirtualGamePad::Initialize()
{
	Int32 loop_cnt_dev;
	Int32 loop_cnt_btn;
	Int32 loop_cnt_evt;

	for (loop_cnt_dev = SNVirtualGamePadIDTop; loop_cnt_dev < SNVirtualGamePadIDNum; loop_cnt_dev++)
	{
		EventExist[loop_cnt_dev] = false;

		// ボタン数ループ
		for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
		{
			// 初期化	
			ButtonState[loop_cnt_dev][loop_cnt_btn].PreviousState = false;
			ButtonState[loop_cnt_dev][loop_cnt_btn].State = false;
			ButtonState[loop_cnt_dev][loop_cnt_btn].RepeatTimer.Stop();
			ButtonState[loop_cnt_dev][loop_cnt_btn].LongPressTimer.Stop();

			for (loop_cnt_evt = SNVirtualGamePadEventTop; loop_cnt_evt < SNVirtualGamePadEventNum; loop_cnt_evt++)
			{
				Event[loop_cnt_dev][loop_cnt_btn][loop_cnt_evt] = false;
			}
		}
	}
	return;
}

// 終了処理
Void SNVirtualGamePad::Terminate()
{
	return;
}

// 状態更新
Void SNVirtualGamePad::Update()
{
	// 前状態更新
	UpdatePrevState();

	// ボタン状態更新
	UpdateButtonState();

	// ボタンイベント更新
	UpdateButtonEvent();

	return;
}

// ボタン状態更新
Void SNVirtualGamePad::UpdateButtonState()
{
	// ゲームパッドから状態反映
	UpdateButtonStateFromGamePad();

	// キーボードから状態反映
	UpdateButtonStateKeyboard();

	return;
}

// 前状態更新
Void SNVirtualGamePad::UpdatePrevState()
{
	Int32 loop_cnt_vp;
	Int32 loop_cnt_btn;
	Int32 loop_cnt_evt;

	// 仮想ゲームパッド数ループ
	for (loop_cnt_vp = SNVirtualGamePadIDTop; loop_cnt_vp < SNVirtualGamePadIDNum; loop_cnt_vp++)
	{
		// ボタン数ループ
		for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
		{
			// 前状態を更新
			ButtonState[loop_cnt_vp][loop_cnt_btn].PreviousState = ButtonState[loop_cnt_vp][loop_cnt_btn].State;

			// 現在状態を初期化
			ButtonState[loop_cnt_vp][loop_cnt_btn].State = false;

			// イベント初期化
			for (loop_cnt_evt = SNVirtualGamePadEventTop; loop_cnt_evt < SNVirtualGamePadEventNum; loop_cnt_evt++)
			{
				Event[loop_cnt_vp][loop_cnt_btn][loop_cnt_evt] = false;
			}

			EventExist[loop_cnt_vp] = false;
		}
	}

	return;
}

// ゲームパッド反映
Void SNVirtualGamePad::UpdateButtonStateFromGamePad()
{
	Int32 loop_cnt_vp;
	Int32 loop_cnt_btn;
	SNGamePadID pad_id;
	SNGamePadButton btn_id;

	// 仮想ゲームパッド数ループ
	for (loop_cnt_vp = SNVirtualGamePadIDTop; loop_cnt_vp < SNVirtualGamePadIDNum; loop_cnt_vp++)
	{
		// ゲームパッドの割り当てを参照
		pad_id = SNUserConfig::Data.GamePadSelect[loop_cnt_vp];

		// 割り当て有効
		if (pad_id != SNGamePadIDNull)
		{
			// ボタン数ループ
			for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
			{
				// 実パッドのボタンID取得
				btn_id = SNUserConfig::Data.GamePadMapping[loop_cnt_vp][loop_cnt_btn];

				// ボタン割り当て有効
				if (btn_id != SNGamePadButtonNull)
				{
					ButtonState[loop_cnt_vp][loop_cnt_btn].State |= SNGamePad::ButtonState[pad_id][btn_id];
				}
			}
		}
	}
	return;
}

// キーボード反映
Void SNVirtualGamePad::UpdateButtonStateKeyboard()
{
	Int32 loop_cnt_vp;
	Int32 loop_cnt_btn;
	SNKeyCode key_code;

	// 仮想ゲームパッド数ループ
	for (loop_cnt_vp = SNVirtualGamePadIDTop; loop_cnt_vp < SNVirtualGamePadIDNum; loop_cnt_vp++)
	{
		// ボタン数ループ
		for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
		{
			// キー割り当て取得
			key_code = SNUserConfig::Data.KeyboardMapping[loop_cnt_vp][loop_cnt_btn];

			// キー割り当て有効
			if (key_code != SNKeyCodeNull)
			{
				ButtonState[loop_cnt_vp][loop_cnt_btn].State |= SNKeyboard::KeyState[key_code];
			}
		}
	}

	return;
}

// ボタンイベント更新
Void SNVirtualGamePad::UpdateButtonEvent()
{
	Int32 loop_cnt_vp;
	SNInputVirtualDeviceType dev_id[SNVirtualGamePadIDNum] = { SNInputVirtualTypeGamePad1, SNInputVirtualTypeGamePad2 };

	// 仮想ゲームパッド数ループ
	for (loop_cnt_vp = SNVirtualGamePadIDTop; loop_cnt_vp < SNVirtualGamePadIDNum; loop_cnt_vp++)
	{
		UpdateEventPadID((SNVirtualGamePadID)loop_cnt_vp);
	}

	return;
}

// ボタンイベント更新(パッドID指定)
Void SNVirtualGamePad::UpdateEventPadID(SNVirtualGamePadID pad_id)
{
	Int32 loop_cnt_btn;
	UInt32 longpress_time = SNSystemConfig::KeyLongPressTime;
	UInt32 repeat_time = SNSystemConfig::KeyRepeatTime;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// 現在状態、前状態からイベント生成
		// 前状態=OFF
		if (!ButtonState[pad_id][loop_cnt_btn].PreviousState)
		{
			// ON
			if (ButtonState[pad_id][loop_cnt_btn].State)
			{
				Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventPush] = true;
				Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventPress] = true;
				EventExist[pad_id] = true;

				// 長押しタイマ起動
				ButtonState[pad_id][loop_cnt_btn].LongPressTimer.Start((UInt16)longpress_time);
			}

			// OFF
			else
			{
				// OFF中は処理なし
			}
		}
		// 前状態=ON
		else
		{
			// ON
			if (ButtonState[pad_id][loop_cnt_btn].State)
			{
				Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventPress] = true;
				EventExist[pad_id] = true;

				// 長押し時間経過
				if (ButtonState[pad_id][loop_cnt_btn].LongPressTimer.IsTimeout())
				{
					Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventLongPress] = true;
					EventExist[pad_id] = true;

					// 長押しタイマ停止
					ButtonState[pad_id][loop_cnt_btn].LongPressTimer.Stop();

					// リピートタイマ起動
					ButtonState[pad_id][loop_cnt_btn].RepeatTimer.Start((UInt16)repeat_time);
				}

				// リピート時間経過
				if (ButtonState[pad_id][loop_cnt_btn].RepeatTimer.IsTimeout())
				{
					Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventRepeat] = true;
					EventExist[pad_id] = true;

					// リピートタイマリスタート
					ButtonState[pad_id][loop_cnt_btn].RepeatTimer.Restart();
				}
			}

			// OFF
			else
			{
				Event[pad_id][loop_cnt_btn][SNVirtualGamePadEventRelease] = true;
				EventExist[pad_id] = true;

				// タイマ停止
				ButtonState[pad_id][loop_cnt_btn].LongPressTimer.Stop();
				ButtonState[pad_id][loop_cnt_btn].RepeatTimer.Stop();
			}
		}
	}

	return;
}
