#include "SNVirtualGamePad.h"
#include "SNConfig.h"
#include "SNInput.h"
#include "SNGamePad.h"
#include "SNKeyboard.h"

Boolean SNVirtualGamePad::Event[SNVirtualGamePadButtonNum][SNVirtualGamePadEventNum];
SNVirtualGamePadButtonState SNVirtualGamePad::ButtonState[SNVirtualGamePadButtonNum];
Boolean SNVirtualGamePad::EventExist;


// 初期化
Void SNVirtualGamePad::Initialize()
{
	Int32 loop_cnt_btn;
	Int32 loop_cnt_evt;

	EventExist = false;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// 初期化	
		ButtonState[loop_cnt_btn].PreviousState = false;
		ButtonState[loop_cnt_btn].State = false;
		ButtonState[loop_cnt_btn].RepeatTimer.Stop();
		ButtonState[loop_cnt_btn].LongPressTimer.Stop();

		for (loop_cnt_evt = SNVirtualGamePadEventTop; loop_cnt_evt < SNVirtualGamePadEventNum; loop_cnt_evt++)
		{
			Event[loop_cnt_btn][loop_cnt_evt] = false;
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
	Int32 loop_cnt_btn;
	Int32 loop_cnt_evt;

	EventExist = false;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// 前状態を更新
		ButtonState[loop_cnt_btn].PreviousState = ButtonState[loop_cnt_btn].State;

		// 現在状態を初期化
		ButtonState[loop_cnt_btn].State = false;

		// イベント初期化
		for (loop_cnt_evt = SNVirtualGamePadEventTop; loop_cnt_evt < SNVirtualGamePadEventNum; loop_cnt_evt++)
		{
			Event[loop_cnt_btn][loop_cnt_evt] = false;
		}
	}

	return;
}

// ゲームパッド反映
Void SNVirtualGamePad::UpdateButtonStateFromGamePad()
{
	Int32 loop_cnt_btn;
	SNGamePadButton btn_id;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// 実パッドのボタンID取得
		btn_id = SNUserConfig::Data.GamePadMapping[loop_cnt_btn];

		// ボタン割り当て有効
		if (btn_id != SNGamePadButtonNull)
		{
			ButtonState[loop_cnt_btn].State |= SNGamePad::ButtonState[btn_id];
		}
	}
	return;
}

// キーボード反映
Void SNVirtualGamePad::UpdateButtonStateKeyboard()
{
	Int32 loop_cnt_btn;
	SNKeyCode key_code;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// キー割り当て取得
		key_code = SNUserConfig::Data.KeyboardMapping[loop_cnt_btn];

		// キー割り当て有効
		if (key_code != SNKeyCodeNull)
		{
			ButtonState[loop_cnt_btn].State |= SNKeyboard::KeyState[key_code];
		}
	}

	return;
}

// ボタンイベント更新
Void SNVirtualGamePad::UpdateButtonEvent()
{
	Int32 loop_cnt_btn;
	UInt32 longpress_time = SNSystemConfig::KeyLongPressTime;
	UInt32 repeat_time = SNSystemConfig::KeyRepeatTime;

	// ボタン数ループ
	for (loop_cnt_btn = SNVirtualGamePadTop; loop_cnt_btn < SNVirtualGamePadButtonNum; loop_cnt_btn++)
	{
		// 現在状態、前状態からイベント生成
		// 前状態=OFF
		if (!ButtonState[loop_cnt_btn].PreviousState)
		{
			// ON
			if (ButtonState[loop_cnt_btn].State)
			{
				Event[loop_cnt_btn][SNVirtualGamePadEventPush] = true;
				Event[loop_cnt_btn][SNVirtualGamePadEventPress] = true;
				EventExist = true;

				// 長押しタイマ起動
				ButtonState[loop_cnt_btn].LongPressTimer.Start((UInt16)longpress_time);
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
			if (ButtonState[loop_cnt_btn].State)
			{
				Event[loop_cnt_btn][SNVirtualGamePadEventPress] = true;
				EventExist = true;

				// 長押し時間経過
				if (ButtonState[loop_cnt_btn].LongPressTimer.IsTimeout())
				{
					Event[loop_cnt_btn][SNVirtualGamePadEventLongPress] = true;
					EventExist = true;

					// 長押しタイマ停止
					ButtonState[loop_cnt_btn].LongPressTimer.Stop();

					// リピートタイマ起動
					ButtonState[loop_cnt_btn].RepeatTimer.Start((UInt16)repeat_time);
				}

				// リピート時間経過
				if (ButtonState[loop_cnt_btn].RepeatTimer.IsTimeout())
				{
					Event[loop_cnt_btn][SNVirtualGamePadEventRepeat] = true;
					EventExist = true;

					// リピートタイマリスタート
					ButtonState[loop_cnt_btn].RepeatTimer.Restart();
				}
			}

			// OFF
			else
			{
				Event[loop_cnt_btn][SNVirtualGamePadEventRelease] = true;
				EventExist = true;

				// タイマ停止
				ButtonState[loop_cnt_btn].LongPressTimer.Stop();
				ButtonState[loop_cnt_btn].RepeatTimer.Stop();
			}
		}
	}

	return;
}


// Up
Boolean SNVirtualGamePad::DPadUpPush()
{
	return Event[SNVirtualGamePadUp][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::DPadUpPress()
{
	return Event[SNVirtualGamePadUp][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::DPadUpRepeat()
{
	return Event[SNVirtualGamePadUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::DPadUpLong()
{
	return Event[SNVirtualGamePadUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::DPadUpRelease()
{
	return Event[SNVirtualGamePadUp][SNVirtualGamePadEventRelease];
}

// Down
Boolean SNVirtualGamePad::DPadDownPush()
{
	return Event[SNVirtualGamePadDown][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::DPadDownPress()
{
	return Event[SNVirtualGamePadDown][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::DPadDownRepeat()
{
	return Event[SNVirtualGamePadDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::DPadDownLong()
{
	return Event[SNVirtualGamePadDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::DPadDownRelease()
{
	return Event[SNVirtualGamePadDown][SNVirtualGamePadEventRelease];
}

// Left
Boolean SNVirtualGamePad::DPadLeftPush()
{
	return Event[SNVirtualGamePadLeft][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::DPadLeftPress()
{
	return Event[SNVirtualGamePadLeft][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::DPadLeftRepeat()
{
	return Event[SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::DPadLeftLong()
{
	return Event[SNVirtualGamePadLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::DPadLeftRelease()
{
	return Event[SNVirtualGamePadLeft][SNVirtualGamePadEventRelease];
}

// Right
Boolean SNVirtualGamePad::DPadRightPush()
{
	return Event[SNVirtualGamePadRight][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::DPadRightPress()
{
	return Event[SNVirtualGamePadRight][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::DPadRightRepeat()
{
	return Event[SNVirtualGamePadRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::DPadRightLong()
{
	return Event[SNVirtualGamePadRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::DPadRightRelease()
{
	return Event[SNVirtualGamePadRight][SNVirtualGamePadEventRelease];
}

// A
Boolean SNVirtualGamePad::APush()
{
	return Event[SNVirtualGamePadDecide][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::APress()
{
	return Event[SNVirtualGamePadDecide][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::ARepeat()
{
	return Event[SNVirtualGamePadDecide][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::APadALong()
{
	return Event[SNVirtualGamePadDecide][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::APadARelease()
{
	return Event[SNVirtualGamePadDecide][SNVirtualGamePadEventRelease];
}

// B
Boolean SNVirtualGamePad::BPush()
{
	return Event[SNVirtualGamePadCancel][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::BPress()
{
	return Event[SNVirtualGamePadCancel][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::BRepeat()
{
	return Event[SNVirtualGamePadCancel][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::BLong()
{
	return Event[SNVirtualGamePadCancel][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::BRelease()
{
	return Event[SNVirtualGamePadCancel][SNVirtualGamePadEventRelease];
}

// X
Boolean SNVirtualGamePad::XPush()
{
	return Event[SNVirtualGamePadMenu][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::XPress()
{
	return Event[SNVirtualGamePadMenu][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::XRepeat()
{
	return Event[SNVirtualGamePadMenu][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::XLong()
{
	return Event[SNVirtualGamePadMenu][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::XRelease()
{
	return Event[SNVirtualGamePadMenu][SNVirtualGamePadEventRelease];
}

// Y
Boolean SNVirtualGamePad::YPush()
{
	return Event[SNVirtualGamePadAction][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::YPress()
{
	return Event[SNVirtualGamePadAction][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::YRepeat()
{
	return Event[SNVirtualGamePadAction][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::YLong()
{
	return Event[SNVirtualGamePadAction][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::YRelease()
{
	return Event[SNVirtualGamePadAction][SNVirtualGamePadEventRelease];
}

// Select
Boolean SNVirtualGamePad::SelectPush()
{
	return Event[SNVirtualGamePadSelect][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::SelectPress()
{
	return Event[SNVirtualGamePadSelect][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::SelectRepeat()
{
	return Event[SNVirtualGamePadSelect][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::SelectLong()
{
	return Event[SNVirtualGamePadSelect][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::SelectRelease()
{
	return Event[SNVirtualGamePadSelect][SNVirtualGamePadEventRelease];
}

// Start
Boolean SNVirtualGamePad::StartPush()
{
	return Event[SNVirtualGamePadStart][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::StartPress()
{
	return Event[SNVirtualGamePadStart][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::StartRepeat()
{
	return Event[SNVirtualGamePadStart][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::StartLong()
{
	return Event[SNVirtualGamePadStart][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::StartRelease()
{
	return Event[SNVirtualGamePadStart][SNVirtualGamePadEventRelease];
}

// L
Boolean SNVirtualGamePad::L1Push()
{
	return Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::L1Press()
{
	return Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::L1Repeat()
{
	return Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::L1Long()
{
	return Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::L1Release()
{
	return Event[SNVirtualGamePadPagePrev][SNVirtualGamePadEventRelease];
}

// R
Boolean SNVirtualGamePad::R1Push()
{
	return Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::R1Press()
{
	return Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::R1Repeat()
{
	return Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::R1Long()
{
	return Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::R1Release()
{
	return Event[SNVirtualGamePadPageNext][SNVirtualGamePadEventRelease];
}

// L2
Boolean SNVirtualGamePad::L2Push()
{
	return Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::L2Press()
{
	return Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::L2Repeat()
{
	return Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::L2Long()
{
	return Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::L2Release()
{
	return Event[SNVirtualGamePadTriggerL][SNVirtualGamePadEventRelease];
}

// R2
Boolean SNVirtualGamePad::R2Push()
{
	return Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::R2Press()
{
	return Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::R2Repeat()
{
	return Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::R2Long()
{
	return Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::R2Release()
{
	return Event[SNVirtualGamePadTriggerR][SNVirtualGamePadEventRelease];
}

// L3
Boolean SNVirtualGamePad::L3Push()
{
	return Event[SNVirtualGamePadL3][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::L3Press()
{
	return Event[SNVirtualGamePadL3][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::L3Repeat()
{
	return Event[SNVirtualGamePadL3][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::L3Long()
{
	return Event[SNVirtualGamePadL3][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::L3Release()
{
	return Event[SNVirtualGamePadL3][SNVirtualGamePadEventRelease];
}

// R3
Boolean SNVirtualGamePad::R3Push()
{
	return Event[SNVirtualGamePadR3][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::R3Press()
{
	return Event[SNVirtualGamePadR3][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::R3Repeat()
{
	return Event[SNVirtualGamePadR3][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::R3Long()
{
	return Event[SNVirtualGamePadR3][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::R3Release()
{
	return Event[SNVirtualGamePadR3][SNVirtualGamePadEventRelease];
}

// L Stick Up
Boolean SNVirtualGamePad::LStkUpPush()
{
	return Event[SNVirtualGamePadListUp][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::LStkUpPress()
{
	return Event[SNVirtualGamePadListUp][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::LStkUpRepeat()
{
	return Event[SNVirtualGamePadListUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::LStkUpLong()
{
	return Event[SNVirtualGamePadListUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::LStkUpRelease()
{
	return Event[SNVirtualGamePadListUp][SNVirtualGamePadEventRelease];
}

// L Stick Down
Boolean SNVirtualGamePad::LStkDownPush()
{
	return Event[SNVirtualGamePadListDown][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::LStkDownPress()
{
	return Event[SNVirtualGamePadListDown][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::LStkDownRepeat()
{
	return Event[SNVirtualGamePadListDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::LStkDownLong()
{
	return Event[SNVirtualGamePadListDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::LStkDownRelease()
{
	return Event[SNVirtualGamePadListDown][SNVirtualGamePadEventRelease];
}

// L Stick Left
Boolean SNVirtualGamePad::LStkLeftPush()
{
	return Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::LStkLeftPress()
{
	return Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::LStkLeftRepeat()
{
	return Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::LStkLeftLong()
{
	return Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::LStkLeftRelease()
{
	return Event[SNVirtualGamePadLStkLeft][SNVirtualGamePadEventRelease];
}

// L Stick Right
Boolean SNVirtualGamePad::LStkRightPush()
{
	return Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::LStkRightPress()
{
	return Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::LStkRightRepeat()
{
	return Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::LStkRightLong()
{
	return Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::LStkRightRelease()
{
	return Event[SNVirtualGamePadLStkRight][SNVirtualGamePadEventRelease];
}

// R Stick Up
Boolean SNVirtualGamePad::RStkUpPush()
{
	return Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::RStkUpPress()
{
	return Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::RStkUpRepeat()
{
	return Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::RStkUpLong()
{
	return Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::RStkUpRelease()
{
	return Event[SNVirtualGamePadRStkUp][SNVirtualGamePadEventRelease];
}

// R Stick Down
Boolean SNVirtualGamePad::RStkDownPush()
{
	return Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::RStkDownPress()
{
	return Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::RStkDownRepeat()
{
	return Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::RStkDownLong()
{
	return Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::RStkDownRelease()
{
	return Event[SNVirtualGamePadRStkDown][SNVirtualGamePadEventRelease];
}

// R Stick Left
Boolean SNVirtualGamePad::RStkLeftPush()
{
	return Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::RStkLeftPress()
{
	return Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::RStkLeftRepeat()
{
	return Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::RStkLeftLong()
{
	return Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::RStkLeftRelease()
{
	return Event[SNVirtualGamePadRStkLeft][SNVirtualGamePadEventRelease];
}

// R Stick Right
Boolean SNVirtualGamePad::RStkRightPush()
{
	return Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventPush];
}
Boolean SNVirtualGamePad::RStkRightPress()
{
	return Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventPress];
}
Boolean SNVirtualGamePad::RStkRightRepeat()
{
	return Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventRepeat];
}
Boolean SNVirtualGamePad::RStkRightLong()
{
	return Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventLongPress];
}
Boolean SNVirtualGamePad::RStkRightRelease()
{
	return Event[SNVirtualGamePadRStkRight][SNVirtualGamePadEventRelease];
}

