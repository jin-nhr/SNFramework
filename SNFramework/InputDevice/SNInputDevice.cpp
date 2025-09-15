#include "SNInputDevice.h"
#include "../Configuration/SNConfiguration.h"
#include "../Application/SNApplication.h"

// 入力装置クラス

// アクティブ状態
Boolean SNInputDevice::Active = false;

// 入力イベント
SNInputDeviceEvent SNInputDevice::InputDeviceEvent = { 0 };

// 物理入力イベント
SNPhysicalInputEvent SNInputDevice::PhysicalInputEvent = { 0 };

// ボタン状態
SNInputButtonState SNInputDevice::ButtonState[SNInputDeviceNum][SNInputButtonNum] = { 0 };

// ポインティングイベント判定状態
SNInputPointingState SNInputDevice::PointingState = { 0 };


// 初期化処理
Void SNInputDevice::Initialize()
{
	// 配下デバイスの初期化
	SNKeyboard::Initialize();
	SNMouse::Initialize();
	SNGamePad::Initialize();

	return;
}

// 起動準備
Void SNInputDevice::Startup()
{
	return;
}

// 実行
// リターン：終了コード
Int32 SNInputDevice::Run()
{
	return 0;
}

// 終了前処理
Void SNInputDevice::BeforeTerminate()
{
	return;
}

// 終了
Void SNInputDevice::Terminate()
{
	// 配下デバイスの終了処理
	SNKeyboard::Terminate();
	SNMouse::Terminate();
	SNGamePad::Terminate();

	return;
}

// 更新処理
Void SNInputDevice::Update()
{
	// 状態更新
	UpdateState();

	// デバイス入力更新
	UpdateDevice();

	// 物理デバイス入力更新
	UpdatePhysicalInput();

	// ボタンイベント生成
	GenerateButtonEvent();

	// ポインティングイベント生成
	GeneratePointingEvent();

	return;
}

// 入力イベント取得
const SNInputDeviceEvent* SNInputDevice::GetInputDeviceEvent()
{
	return &InputDeviceEvent;
}

// 物理デバイス情報取得
const SNPhysicalInputEvent* SNInputDevice::GetPhysicalInputEvent()
{
	return &PhysicalInputEvent;
}

// 状態更新
Void SNInputDevice::UpdateState()
{
	// アクティブ通知あり
	if (SNApplication::GetEventInfo()->Active)
	{
		Active = true;
	}

	// 非アクティブ通知あり
	else if (SNApplication::GetEventInfo()->NonActive)
	{
		Active = false;
	}

	// 通知なし
	else
	{
		// 現状態継続
	}

	return;
}

// デバイス入力情報更新
Void SNInputDevice::UpdateDevice()
{
	// キーボード入力状態更新
	SNKeyboard::Update();

	// マウス入力状態更新
	SNMouse::Update();

	// ゲームパッド入力状態更新
	SNGamePad::Update();

	return;
}

// 物理デバイス入力情報更新
Void SNInputDevice::UpdatePhysicalInput()
{
	UInt32 cnt;

	// アクティブの場合は各デバイスの情報を取得
	if (Active)
	{
		// キーボード物理入力イベントセット
		for (cnt = 0; cnt < SNKeyboard::CheckKeyNum; cnt++)
		{
			PhysicalInputEvent.Keyboard[cnt] = SNKeyboard::KeyState[SNKeyboard::CheckKeyList[cnt]];
		}

		// マウス物理入力イベントセット
		for (cnt = 0; cnt < SNMouseButtonNum; cnt++)
		{
			PhysicalInputEvent.MouseButton[cnt] = SNMouse::MouseState.ButtonState[cnt];
		}
		PhysicalInputEvent.MousePosition = SNMouse::MouseState.Position;
		PhysicalInputEvent.MouseClipping = SNMouse::MouseState.PosClipping;

		// ゲームパッド
		for (cnt = 0; cnt < SNGamePadButtonNum; cnt++)
		{
			PhysicalInputEvent.GamePad[SNGamePadID1][cnt] = SNGamePad::ButtonState[SNGamePadID1][cnt];
			PhysicalInputEvent.GamePad[SNGamePadID2][cnt] = SNGamePad::ButtonState[SNGamePadID2][cnt];
		}
	}

	// 非アクティブのときは各ボタン状態等を強制解放
	else
	{
		// キーボード物理入力イベントセット
		for (cnt = 0; cnt < SNKeyboard::CheckKeyNum; cnt++)
		{
			PhysicalInputEvent.Keyboard[cnt] = false;
		}

		// マウス物理入力イベントセット
		for (cnt = 0; cnt < SNMouseButtonNum; cnt++)
		{
			PhysicalInputEvent.MouseButton[cnt] = false;
		}
		//PhysicalInputEvent.MousePosition = 前状態を継続
		PhysicalInputEvent.MouseClipping = false;

		// ゲームパッド
		for (cnt = 0; cnt < SNGamePadButtonNum; cnt++)
		{
			PhysicalInputEvent.GamePad[SNGamePadID1][cnt] = false;
			PhysicalInputEvent.GamePad[SNGamePadID2][cnt] = false;
		}
	}

	return;
}

// ボタンイベント生成
Void SNInputDevice::GenerateButtonEvent()
{
	Int32 loop_cnt_dev;
	Int32 loop_cnt_pad;
	Int32 loop_cnt_btn;
	SNInputMapping* mapping = SNConfiguration::UserConfiguration.InputMapping;
	UInt32 longpress_time = SNConfiguration::SystemConfiguration.KeyLongPressTime;
	UInt32 repeart_time = SNConfiguration::SystemConfiguration.KeyRepeatTime;

	// 入力デバイス数ループ
	for (loop_cnt_dev = 0; loop_cnt_dev < SNInputDeviceNum; loop_cnt_dev++)
	{
		// ボタン数ループ
		for (loop_cnt_btn = 0; loop_cnt_btn < SNInputButtonNum; loop_cnt_btn++)
		{
			// 前状態を更新
			ButtonState[loop_cnt_dev][loop_cnt_btn].PreviousState = ButtonState[loop_cnt_dev][loop_cnt_btn].State;
			
			// 現在状態を初期化
			ButtonState[loop_cnt_dev][loop_cnt_btn].State = false;

			// キーボード判定
			if (mapping[loop_cnt_dev].KeyboadMapping[loop_cnt_btn] != SNKeyCodeNull)
			{
				ButtonState[loop_cnt_dev][loop_cnt_btn].State |= PhysicalInputEvent.Keyboard[mapping[loop_cnt_dev].KeyboadMapping[loop_cnt_btn]];
			}

			// マウス判定
			if (mapping[loop_cnt_dev].MouseMapping[loop_cnt_btn] != SNMouseButtonNull)
			{
				ButtonState[loop_cnt_dev][loop_cnt_btn].State |= PhysicalInputEvent.MouseButton[mapping[loop_cnt_dev].MouseMapping[loop_cnt_btn]];
			}

			// ゲームパッド判定
			for (loop_cnt_pad = 0; loop_cnt_pad < SNGamePadIDNum; loop_cnt_pad++)
			{
				if (mapping[loop_cnt_dev].GamePadMapping[loop_cnt_pad][loop_cnt_btn] != SNGamePadButtonNull)
				{
					ButtonState[loop_cnt_dev][loop_cnt_btn].State |= PhysicalInputEvent.GamePad[loop_cnt_dev][mapping[loop_cnt_dev].GamePadMapping[loop_cnt_pad][loop_cnt_btn]];
				}
			}

			// イベント初期化
			InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn] = {0};

			// 現在状態、前状態からイベント生成
			// 前状態=OFF
			if (!ButtonState[loop_cnt_dev][loop_cnt_btn].PreviousState)
			{
				// ON
				if (ButtonState[loop_cnt_dev][loop_cnt_btn].State)
				{
					InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].Push = true;
					InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].Press = true;
					
					// 長押しタイマ起動
					ButtonState[loop_cnt_dev][loop_cnt_btn].LongPressTimer.Start((UInt16)longpress_time);
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
				if (ButtonState[loop_cnt_dev][loop_cnt_btn].State)
				{
					InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].Press = true;

					// 長押し時間経過
					if (ButtonState[loop_cnt_dev][loop_cnt_btn].LongPressTimer.IsTimeout())
					{
						InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].LongPress = true;

						// 長押しタイマ停止
						ButtonState[loop_cnt_dev][loop_cnt_btn].LongPressTimer.Stop();

						// リピートタイマ起動
						ButtonState[loop_cnt_dev][loop_cnt_btn].RepeatTimer.Start((UInt16)repeart_time);
					}

					// リピート時間経過
					if (ButtonState[loop_cnt_dev][loop_cnt_btn].RepeatTimer.IsTimeout())
					{
						InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].Repeat = true;

						// リピートタイマリスタート
						ButtonState[loop_cnt_dev][loop_cnt_btn].RepeatTimer.Restart();
					}
				}

				// OFF
				else
				{
					InputDeviceEvent.ButtonEvent[loop_cnt_dev][loop_cnt_btn].Release = true;

					// タイマ停止
					ButtonState[loop_cnt_dev][loop_cnt_btn].LongPressTimer.Stop();
					ButtonState[loop_cnt_dev][loop_cnt_btn].RepeatTimer.Stop();
				}
			}
		}
	}

	return;
}

// ポインティングイベント生成
Void SNInputDevice::GeneratePointingEvent()
{
	SNPoint point = PhysicalInputEvent.MousePosition;
	Boolean decide = PhysicalInputEvent.MouseButton[SNMouseButtonDecide];
	Boolean cancel = PhysicalInputEvent.MouseButton[SNMouseButtonCancel];
	Boolean wup = PhysicalInputEvent.MouseButton[SNMouseButtonWheelUp];
	Boolean wdown = PhysicalInputEvent.MouseButton[SNMouseButtonWheelDown];
	Boolean previous_drag = InputDeviceEvent.PointingEvent.Drag;
	Boolean previous_frick = InputDeviceEvent.PointingEvent.Frick;
	Boolean clipping = PhysicalInputEvent.MouseClipping;
	UInt8 trace_gain = (UInt8)SNConfiguration::SystemConfiguration.FrickTraceGain;
	UInt8 fade_gain = (UInt8)SNConfiguration::SystemConfiguration.FrickFadeGain;

	// イベント初期化
	InputDeviceEvent.PointingEvent = { 0 };

	// 前情報保存
	PointingState.PreviousDecide = PointingState.Decide;
	PointingState.PreviousCancel = PointingState.Cancel;
	InputDeviceEvent.PointingInfo.PreviousPosition = InputDeviceEvent.PointingInfo.CurrentPosition;

	// 現在情報を設定
	PointingState.Decide = decide;
	PointingState.Cancel = cancel;
	InputDeviceEvent.PointingInfo.CurrentPosition = point;

	// 差分情報
	InputDeviceEvent.PointingInfo.DeltaPosition.X = InputDeviceEvent.PointingInfo.CurrentPosition.X - InputDeviceEvent.PointingInfo.PreviousPosition.X;
	InputDeviceEvent.PointingInfo.DeltaPosition.Y = InputDeviceEvent.PointingInfo.CurrentPosition.Y - InputDeviceEvent.PointingInfo.PreviousPosition.Y;

	/////////////////////////////////////////////////
	// 決定判定
	// 前状態OFF→ON
	if ((!PointingState.PreviousDecide && PointingState.Decide) && !clipping)
	{
		InputDeviceEvent.PointingEvent.Decide = true;
		PointingState.ClickEnable = true;

		// ドラッグ、フリック開始点を保存
		InputDeviceEvent.PointingInfo.DragStartPosition = point;

		// フリック関連情報を初期化
		InputDeviceEvent.PointingInfo.FrickVelocity.X = 0;
		InputDeviceEvent.PointingInfo.FrickVelocity.Y = 0;
	}

	// 決定OFF判定
	if (!PointingState.Decide)
	{
		PointingState.ClickEnable = false;
	}

	/////////////////////////////////////////////////
	// キャンセル判定
	// 前状態OFF→ON
	if ((!PointingState.PreviousCancel && PointingState.Cancel) && !clipping)
	{
		InputDeviceEvent.PointingEvent.Cancel = true;
	}

	/////////////////////////////////////////////////
	// ホイール
	InputDeviceEvent.PointingEvent.WheelUp = wup;
	InputDeviceEvent.PointingEvent.WheelDown = wdown;

	/////////////////////////////////////////////////
	// 移動判定
	// X or Yの移動検知
	if ((InputDeviceEvent.PointingInfo.DeltaPosition.X != 0) ||
		(InputDeviceEvent.PointingInfo.DeltaPosition.Y != 0))
	{
		InputDeviceEvent.PointingEvent.Move = true;
	}

	/////////////////////////////////////////////////
	// ドラッグ判定
	// ON継続中の移動検知またはドラッグ
	// Click有効時のみ
	if (PointingState.ClickEnable &&
		PointingState.Decide &&
		PointingState.PreviousDecide &&
		(InputDeviceEvent.PointingEvent.Move || previous_drag))
	{
		InputDeviceEvent.PointingEvent.Drag = true;
	}

	/////////////////////////////////////////////////
	// ドロップ判定
	// ON→OFF検知かつDrag中
	if (!PointingState.Decide &&
		PointingState.PreviousDecide &&
		previous_drag)
	{
		InputDeviceEvent.PointingEvent.Drop = true;
	}

	/////////////////////////////////////////////////
	// フリック開始までの判定

	// フリック動作中以外
	if (!previous_frick)
	{
		// ドラッグ中かつ移動中のみ計算
		if (InputDeviceEvent.PointingEvent.Drag)
		{
			InputDeviceEvent.PointingInfo.FrickVelocity.X = ((InputDeviceEvent.PointingInfo.FrickVelocity.X * trace_gain) / 256) + InputDeviceEvent.PointingInfo.DeltaPosition.X;
			InputDeviceEvent.PointingInfo.FrickVelocity.Y = ((InputDeviceEvent.PointingInfo.FrickVelocity.Y * trace_gain) / 256) + InputDeviceEvent.PointingInfo.DeltaPosition.Y;
		}

		// ドラッグ以外のときは初期化(ドロップ時除く)
		else if (!InputDeviceEvent.PointingEvent.Drop)
		{
			InputDeviceEvent.PointingInfo.FrickVelocity.X = 0;
			InputDeviceEvent.PointingInfo.FrickVelocity.Y = 0;
		}

		// ドロップ時(フリックによる移動開始)
		else
		{
			InputDeviceEvent.PointingEvent.Frick = true;
			InputDeviceEvent.PointingInfo.FrickVelocity.X = ((InputDeviceEvent.PointingInfo.FrickVelocity.X * trace_gain) / 256) + InputDeviceEvent.PointingInfo.DeltaPosition.X;
			InputDeviceEvent.PointingInfo.FrickVelocity.Y = ((InputDeviceEvent.PointingInfo.FrickVelocity.Y * trace_gain) / 256) + InputDeviceEvent.PointingInfo.DeltaPosition.Y;
		}
	}

	// フリック中
	else
	{
		// 移動量減衰
		InputDeviceEvent.PointingInfo.FrickVelocity.X = (InputDeviceEvent.PointingInfo.FrickVelocity.X * fade_gain) / 256;
		InputDeviceEvent.PointingInfo.FrickVelocity.Y = (InputDeviceEvent.PointingInfo.FrickVelocity.Y * fade_gain) / 256;

		// XまたはYの移動量ありならFrick継続
		if ((InputDeviceEvent.PointingInfo.FrickVelocity.X) != 0 || (InputDeviceEvent.PointingInfo.FrickVelocity.Y != 0))
		{
			InputDeviceEvent.PointingEvent.Frick = true;
		}
	}

	// 別の操作が入ったときはFrickを解除
	if (InputDeviceEvent.PointingEvent.Decide ||
		InputDeviceEvent.PointingEvent.Cancel ||
		InputDeviceEvent.PointingEvent.WheelUp ||
		InputDeviceEvent.PointingEvent.WheelDown)
	{
		InputDeviceEvent.PointingEvent.Frick = false;
		InputDeviceEvent.PointingInfo.FrickVelocity.X = 0;
		InputDeviceEvent.PointingInfo.FrickVelocity.Y = 0;
	}

	return;
}
