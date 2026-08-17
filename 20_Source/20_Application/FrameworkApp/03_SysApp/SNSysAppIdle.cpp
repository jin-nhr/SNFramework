#include "SNSysAppIdle.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFocus.h"
#include "SNVirtualGamePad.h"

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

// イベント通知
Boolean SNSysAppIdle::OnNotifyEvent()
{
	// 終了通知
	if (SNApplication::GetEventInfo(SNEventExitApplication))
	{
		TransCode = SNTransitionCode3;

		// 処理対象のイベント種別をキャンセル
		SNEvent::EventExist[SNEventTypeNotifyEvent] = false;

		SNFocus::CallbackPushButton();
	}

	return false;
}

// 内部イベント通知
Boolean SNSysAppIdle::OnInternalEvent()
{
	Boolean ret = false;

	if (SNEvent::InternalEvent[SNEventResultDspPreClose])
	{
		TransCode = SNTransitionCode3;
		ret = true;
	}

	else if (SNEvent::InternalEvent[SNEventResultDspConfig])
	{
		TransCode = SNTransitionCode0;

		ret = true;
	}

	return ret;
}

