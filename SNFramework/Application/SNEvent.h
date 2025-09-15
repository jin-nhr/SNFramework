#pragma once
#include "../Include/SNFramework.h"

// イベントクラス
class SNEvent
{
public:
	// コンストラクタ
	SNEvent();

	// デストラクタ
	virtual ~SNEvent();

	// 初期化
	Void Initialize();

	// 終了処理
	Void Terminate();

	// 更新処理
	Void Update();

	// 入力イベント無効化
	Void InputEventDisable();

	// アプリケーションイベント
	SNApplicationEventInfo ApplicationEvent;

	// 物理入力イベント
	SNPhysicalInputEvent PhysicalInputEvent;

	// 入力イベント有効フラグ
	Boolean InputEventEnable;

	// 入力イベント
	SNInputDeviceEvent InputEvent;

	// ループバックイベント
	SNLoopbackEvent LoopbackEvent;

	// リザルト
	SNEventResult Result;

private:


};
