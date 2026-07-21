#include "SNSysAppIdle.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"

// コンストラクタ
SNSysAppIdle::SNSysAppIdle()
{
	return;
}

// デストラクタ
SNSysAppIdle::~SNSysAppIdle()
{
	return;
}

// Entry
Void SNSysAppIdle::OnEntry()
{
	SNMute::SetMute(false, true);

	return;
}



// フレーム処理
Void SNSysAppIdle::OnCycle()
{
	return;
}

