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

	// アプリケーションイベント
	const SNApplicationEventInfo* ApplicationEvent;

	// 物理入力イベント
	const SNPhysicalInputEvent* PhysicalInputEvent;

	// 入力イベント
	const SNInputDeviceEvent* InputEvent;

	// ループバックイベント
	SNLoopbackEvent LoopbackEvent;

	// リザルト
	SNEventResult Result;

private:


};
