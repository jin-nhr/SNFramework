#include "SNFocus.h"
#include "SNFocusGroup.h"
#include "SNVirtualPointingDevice.h"
#include "SNVirtualGamePad.h"
#include "SNSystemConfig.h"

SNFocusGroup* SNFocus::SysAppGroup = nullptr;
SNFocusGroup* SNFocus::UserAppGroup = nullptr;

SNSoundEffect SNFocus::SEPush;
SNSoundEffect SNFocus::SEFocus;


Void SNFocus::RegisterSysApp(SNFocusGroup* sysapp)
{
	if (SysAppGroup != nullptr)
	{
		// ExitでRelease実行される
		SysAppGroup->Exit();
	}
	
	SysAppGroup = sysapp;

	return;
}
Void SNFocus::ReleaseSysApp(SNFocusGroup* sysapp)
{
	if (SysAppGroup == sysapp)
	{
		SysAppGroup = nullptr;
	}

	// もしレイヤ化する場合は次のGroupをEntryする

	return;
}

Void SNFocus::CallbackPushButton()
{
	SEPush.Play();

	return;
}


Void SNFocus::OnEntry()
{
	SEPush.CreateEffect(SNSoundResButtonPush);
	SEFocus.CreateEffect(SNSoundResFocusMove);

	return;
}

Void SNFocus::OnExit()
{
	SEPush.DeleteEffect();
	SEFocus.DeleteEffect();

	return;
}


Void SNFocus::OnCycle()
{
	if (TimerSeq.IsProc())
	{
		TimerSeq.Step();
	}

	return;
}

Void SNFocus::RegisterUserApp(SNFocusGroup* userapp)
{
	if (UserAppGroup != nullptr)
	{
		// ExitでRelease実行される
		UserAppGroup->Exit();
	}

	UserAppGroup = userapp;

	return;
}
Void SNFocus::ReleaseUserApp(SNFocusGroup* userapp)
{
	if (UserAppGroup == userapp)
	{
		UserAppGroup = nullptr;
	}

	// もしレイヤ化する場合は次のGroupをEntryする

	return;
}


SNFocus::SNFocus()
{
	TimerSeq.Initialize(this, 0, 1);
	TimerSeq.SetWait(0, SNSystemConfig::GUIButtonPushTime);
	return;
}

SNFocus::~SNFocus()
{
	return;
}

SNFocusGroup* SNFocus::GetTarget()
{
	SNFocusGroup* target = nullptr;

	if (SysAppGroup != nullptr)
	{
		target = SysAppGroup;
	}
	else if (UserAppGroup != nullptr)
	{
		target = UserAppGroup;
	}

	return target;
}

Boolean SNFocus::OnPointingDevice()
{
	SNFocusGroup* target = GetTarget();
	Boolean ret = false;
	SNPoint pnt = SNVirtualPointingDevice::Info[SNVirtualPointingInfoCurrent];
	Boolean pnt_push = SNVirtualPointingDevice::Event[SNVirtualPointingEventDecide];
	Boolean cancel_push = SNVirtualPointingDevice::Event[SNVirtualPointingEventCancel];
	SNGUIButton* active_btn = nullptr;

	if (!TimerSeq.IsProc())
	{
		if (target != nullptr)
		{
			target->MovePoint(&pnt);
			active_btn = target->GetActiveButton();

			if (pnt_push)
			{
				target->PushPointingDecide(&pnt);

				if (active_btn != nullptr)
				{
					if (active_btn->Push)
					{
						SEPush.Play();
						TimerSeq.Start();
					}
				}
			}

			if (cancel_push)
			{
				target->PushCancel();
			}

			if (active_btn != target->GetActiveButton())
			{
				SEFocus.Play();
			}

			ret = true;
		}
	}

	return ret;
}

Boolean SNFocus::OnGamePad1()
{
	SNFocusGroup* target = GetTarget();
	Boolean ret = false;

	Boolean decide_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDecide][SNVirtualGamePadEventPush];
	Boolean cancel_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadCancel][SNVirtualGamePadEventPush];

	Boolean up_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventPush];
	Boolean up_rpt  = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventRepeat];
	Boolean down_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventPush];
	Boolean down_rpt = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventRepeat];
	Boolean left_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventPush];
	Boolean left_rpt = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat];
	Boolean right_push = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventPush];
	Boolean right_rpt = SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventRepeat];
	SNGUIButton* active_btn = nullptr;

	if (!TimerSeq.IsProc())
	{
		if (target != nullptr)
		{
			active_btn = target->GetActiveButton();

			if (decide_push)
			{
				target->PushDecide();
				if (active_btn != nullptr)
				{
					if (active_btn->Push)
					{
						SEPush.Play();
						TimerSeq.Start();
					}
				}
			}

			if (cancel_push)
			{
				target->PushCancel();
			}

			if (up_push || up_rpt)
			{
				target->DirUp();
			}

			if (down_push || down_rpt)
			{
				target->DirDown();
			}

			if (left_push || left_rpt)
			{
				target->DirLeft();
			}

			if (right_push || right_rpt)
			{
				target->DirRight();
			}

			if (active_btn != target->GetActiveButton())
			{
				SEFocus.Play();
			}

			ret = true;
		}
	}

	return ret;
}

SNPhaseResult SNFocus::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;
	SNFocusGroup* target = GetTarget();

	switch (ch)
	{
	case 0:
		if (target != nullptr)
		{
			target->ReleaseDecide();
		}
		ret = SNPhaseResultNext;
		break;

	default:
		break;
	}

	return ret;
}
