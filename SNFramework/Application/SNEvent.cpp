#include "SNEvent.h"
#include "SNApplication.h"
#include "../InputDevice/SNInputDevice.h"

// イベントクラス

// コンストラクタ
SNEvent::SNEvent()
{
	// 初期化
	ApplicationEvent = { 0 };
	PhysicalInputEvent = { 0 };
	InputEventEnable = false;
	InputEvent = { 0 };
	LoopbackEvent = {0};
	Result = {0};
	return;
}

// デストラクタ
SNEvent::~SNEvent()
{
	return;
}

// 初期化
Void SNEvent::Initialize()
{
	return;
}

// 終了処理
Void SNEvent::Terminate()
{
	return;
}

// 更新処理
Void SNEvent::Update()
{
	// アプリケーションイベント取得
	ApplicationEvent = *SNApplication::GetEventInfo();

	// 物理入力イベント取得
	PhysicalInputEvent = *SNInputDevice::GetPhysicalInputEvent();

	// 入力イベント取得
	InputEvent = *SNInputDevice::GetInputDeviceEvent();

	// 入力イベント有効化
	InputEventEnable = true;

	// 前回のリザルトを参照し、ループバックイベント設定
	LoopbackEvent = {0};

	// リザルト初期化
	Result = {0};

	return;
}

// 入力イベント無効化
Void SNEvent::InputEventDisable()
{
	InputEventEnable = false;

	return;
}
