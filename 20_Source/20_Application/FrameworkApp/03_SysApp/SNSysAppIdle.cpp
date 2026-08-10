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
	if (SNMute::GetMuteSts() == SNMuteStsOn)
	{
		SNMute::SetMute(false, true);
	}

	return;
}

Boolean SNSysAppIdle::OnGamePad1()
{
	Boolean ret = false;
	Boolean menu = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadMenu][SNVirtualGamePadEventPush];

	if (menu)
	{
		SNFocus::CallbackPushButton();

		TransCode = SNTransitionCode0;

		ret = true;
	}

	return ret;
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
