#include "SNInputDevice.h"
#include "../Configuration/SNConfiguration.h"
#include "../Application/SNApplication.h"

// 入力装置クラス


// 共通メソッド/データ

// インスタンス生成/取得
SNInputDevice* SNInputDevice::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNInputDevice;
	}

	return Me;
}

// インスタンス破棄
Void SNInputDevice::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNInputDevice* SNInputDevice::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNInputDevice::~SNInputDevice()
{

	return;
}


// 初期化処理
Void SNInputDevice::Initialize()
{
	UInt32 loop_cnt;

	// 子クラスインスタンス生成
	Keyboard = new SNKeyboard;
	Mouse = new SNMouse;
	for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
	{
		GamePad[loop_cnt] = new SNGamePad;
	}

	// 配下デバイスの初期化
	Keyboard->Initialize();
	Mouse->Initialize();
	for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
	{
		GamePad[loop_cnt]->Initialize((UInt8)loop_cnt);
	}

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
	UInt32 loop_cnt;

	// 配下デバイスの終了処理
	Keyboard->Terminate();
	Mouse->Terminate();
	for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
	{
		GamePad[loop_cnt]->Terminate();
	}

	delete Keyboard;
	delete Mouse;
	for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
	{
		delete GamePad[loop_cnt];
	}

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

// コンストラクタ
// 外部からのインスタンス生成は禁止
SNInputDevice::SNInputDevice()
{
	Int32 loop_cnt_dev;
	Int32 loop_cnt_btn;

	// 変数初期化
	Active = false;
	InputDeviceEvent = {0};
	PhysicalInputEvent = {0};
	PointingState = {0};

	Keyboard = nullptr;
	Mouse = nullptr;
	for (loop_cnt_dev = 0; loop_cnt_dev < SNInputDeviceNum; loop_cnt_dev++)
	{
		GamePad[loop_cnt_dev] = nullptr;
	}

	// 入力デバイス数ループ
	for (loop_cnt_dev = 0; loop_cnt_dev < SNInputDeviceNum; loop_cnt_dev++)
	{
		// ボタン数ループ
		for (loop_cnt_btn = 0; loop_cnt_btn < SNInputButtonNum; loop_cnt_btn++)
		{
			ButtonState[loop_cnt_dev][loop_cnt_btn].State = false;
			ButtonState[loop_cnt_dev][loop_cnt_btn].PreviousState = false;
		}
	}

	// 非アクティブキー状態
	for (loop_cnt_btn = 0; loop_cnt_btn < SNKeyCodeNum; loop_cnt_btn++)
	{
		NonActiveKeyState[loop_cnt_btn] = false;
	}

	// 非アクティブマウス状態
	for (loop_cnt_btn = 0; loop_cnt_btn < SNMouseButtonNum; loop_cnt_btn++)
	{
		NonActiveMouseButtonState[loop_cnt_btn] = false;
	}

	// 非アクティブ座標
	NonActivePointingPosition.X = 0;
	NonActivePointingPosition.Y = 0;

	// 非アクティブボタン状態
	for (loop_cnt_btn = 0; loop_cnt_btn < SNGamePadButtonNum; loop_cnt_btn++)
	{
		NonActiveGamePadButtonState[loop_cnt_btn] = false;
	}

	return;
}

// 状態更新
Void SNInputDevice::UpdateState()
{
	// アクティブ通知あり
	if (SNApplication::GetInstance()->GetEventInfo()->Active)
	{
		Active = true;
	}

	// 非アクティブ通知あり
	else if (SNApplication::GetInstance()->GetEventInfo()->NonActive)
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
	UInt32 loop_cnt;

	// キーボード入力状態更新
	Keyboard->Update();

	// マウス入力状態更新
	Mouse->Update();

	// ゲームパッド入力状態更新
	for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
	{
		GamePad[loop_cnt]->Update();
	}

	return;
}

// 物理デバイス入力情報更新
Void SNInputDevice::UpdatePhysicalInput()
{
	UInt32 loop_cnt;

	// アクティブの場合は各デバイスの情報を取得
	if (Active)
	{
		// キーボード物理入力イベントセット
		PhysicalInputEvent.Keyboard = Keyboard->GetState();

		// マウス物理入力イベントセット
		PhysicalInputEvent.MouseButton = Mouse->GetState()->ButtonState;
		PhysicalInputEvent.MousePosition = &Mouse->GetState()->Position;

		// ゲームパッド物理入力イベントセット
		for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
		{
			PhysicalInputEvent.GamePad[loop_cnt] = GamePad[loop_cnt]->GetState();
		}
	}

	// 非アクティブのときは各ボタン状態等を強制解放
	else
	{
		// キーボード物理入力イベントセット
		PhysicalInputEvent.Keyboard = NonActiveKeyState;

		// マウス物理入力イベントセット
		PhysicalInputEvent.MouseButton = NonActiveMouseButtonState;
		PhysicalInputEvent.MousePosition = &NonActivePointingPosition;

		// ゲームパッド物理入力イベントセット
		for (loop_cnt = 0; loop_cnt < SNGamePadIDNum; loop_cnt++)
		{
			PhysicalInputEvent.GamePad[loop_cnt] = NonActiveGamePadButtonState;
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
	SNInputMapping* mapping = SNConfiguration::GetInstance()->ConfigurationData.User.InputMapping;
	UInt32 longpress_time = SNConfiguration::GetInstance()->ConfigurationData.User.KeyLongPressTime;
	UInt32 repeart_time = SNConfiguration::GetInstance()->ConfigurationData.User.KeyRepeatTime;

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
	SNPoint point = *PhysicalInputEvent.MousePosition;
	Boolean decide = PhysicalInputEvent.MouseButton[SNMouseButtonDecide];
	Boolean cancel = PhysicalInputEvent.MouseButton[SNMouseButtonCancel];
	Boolean wup = PhysicalInputEvent.MouseButton[SNMouseButtonWheelUp];
	Boolean wdown = PhysicalInputEvent.MouseButton[SNMouseButtonWheelDown];
	Boolean previous_drag = InputDeviceEvent.PointingEvent.Drag;
	Boolean previous_frick = InputDeviceEvent.PointingEvent.Frick;
	UInt8 trace_gain = (UInt8)SNConfiguration::GetInstance()->ConfigurationData.System.FrickTraceGain;
	UInt8 fade_gain = (UInt8)SNConfiguration::GetInstance()->ConfigurationData.System.FrickFadeGain;

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
	if (!PointingState.PreviousDecide && PointingState.Decide)
	{
		InputDeviceEvent.PointingEvent.Decide = true;

		// ドラッグ、フリック開始点を保存
		InputDeviceEvent.PointingInfo.DragStartPosition = point;

		// フリック関連情報を初期化
		InputDeviceEvent.PointingInfo.FrickVelocity.X = 0;
		InputDeviceEvent.PointingInfo.FrickVelocity.Y = 0;
	}

	/////////////////////////////////////////////////
	// キャンセル判定
	// 前状態OFF→ON
	if (!PointingState.PreviousCancel && PointingState.Cancel)
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
	// ON継続で移動検知 or ドラッグ中
	if (PointingState.Decide &&
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
