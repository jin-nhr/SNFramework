#include "SNDebugAppOff.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"

// デバッグアプリ OFF

// コンストラクタ
SNDebugAppOff::SNDebugAppOff()
{
	return;
}

// デストラクタ
SNDebugAppOff::~SNDebugAppOff()
{
	return;
}

Void SNDebugAppOff::OnCycle()
{
	// デバッグ有効？
	if (SNSystemConfig::DebugEnable)
	{
		// システムアプリ起動開始
		if (SNEvent::InternalEvent[SNEventResultSysAppStartup])
		{
			TransCode = SNTransitionCode0; // Run状態へ
		}
	}

	return;
}
