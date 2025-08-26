#include "SNEvent.h"
#include "SNApplication.h"
#include "../InputDevice/SNInputDevice.h"

// イベントクラス

// コンストラクタ
SNEvent::SNEvent()
{
	// 初期化
	ApplicationEvent = nullptr;
	PhysicalInputEvent = nullptr;
	InputEvent = nullptr;
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
	ApplicationEvent = SNApplication::GetInstance()->GetEventInfo();

	// 物理入力イベント取得
	PhysicalInputEvent = SNInputDevice::GetInstance()->GetPhysicalInputEvent();

	// 入ry項イベント取得
	InputEvent = SNInputDevice::GetInstance()->GetInputDeviceEvent();

	// 前回のリザルトを参照し、ループバックイベント設定
	LoopbackEvent = {0};

	// リザルト初期化
	Result = {0};

	return;
}
