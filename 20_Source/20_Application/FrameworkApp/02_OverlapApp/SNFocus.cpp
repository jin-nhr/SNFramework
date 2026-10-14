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

	// 動作中
	if (ActiveSts)
	{
		// グループ登録なし
		if (!SysAppGroup && !UserAppGroup)
		{
			SNEvent::EventResult[SNEventResultWorldRestart] = true;
			ActiveSts = false;
		}
	}

	// 非動作中
	else
	{
		// いずれかのグループ登録あり
		if (SysAppGroup || UserAppGroup)
		{
			SNEvent::EventResult[SNEventResultWorldSuspend] = true;
			ActiveSts = true;
		}
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

	return;
}


SNFocus::SNFocus()
{
	ActiveSts = false;
	TimerSeq.Initialize(this, 0, 2);
	TimerSeq.SetWait(0, SNSystemConfig::GUIButtonPushTime);
	TimerSeq.SetWait(1, 0);
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
	SNPoint* pnt = SNVirtualPointingDevice::CurrentPos();
	Boolean pnt_push = SNVirtualPointingDevice::Decide();
	Boolean cancel_push = SNVirtualPointingDevice::Cancel();
	SNGUIButton* active_btn = nullptr;
	SNGUIButton* push_btn = nullptr;

	if (!TimerSeq.IsProc())
	{
		if (target != nullptr)
		{
			active_btn = target->GetActiveButton();
			target->MovePoint(pnt);

			if (pnt_push)
			{
				target->PushPointingDecide(pnt);

				push_btn = target->GetActiveButton();
				if (push_btn != nullptr)
				{
					if (push_btn->Push)
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
	else
	{
		ret = true;
	}

	return ret;
}

Boolean SNFocus::OnGamePad()
{
	SNFocusGroup* target = GetTarget();
	Boolean ret = false;

	Boolean decide_push = SNVirtualGamePad::APush();
	Boolean cancel_push = SNVirtualGamePad::BPush();

	Boolean up_push = SNVirtualGamePad::DPadUpPush() | SNVirtualGamePad::LStkUpPush();
	Boolean up_rpt = SNVirtualGamePad::DPadUpRepeat() | SNVirtualGamePad::LStkUpRepeat();
	Boolean down_push = SNVirtualGamePad::DPadDownPush() | SNVirtualGamePad::LStkDownPush();
	Boolean down_rpt = SNVirtualGamePad::DPadDownRepeat() | SNVirtualGamePad::LStkDownRepeat();
	Boolean left_push = SNVirtualGamePad::DPadLeftPush() | SNVirtualGamePad::LStkLeftPush();
	Boolean left_rpt = SNVirtualGamePad::DPadLeftRepeat() | SNVirtualGamePad::LStkLeftRepeat();
	Boolean right_push = SNVirtualGamePad::DPadRightPush() | SNVirtualGamePad::LStkRightPush();
	Boolean right_rpt = SNVirtualGamePad::DPadRightRepeat() | SNVirtualGamePad::LStkRightRepeat();
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
	else
	{
		ret = true;
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
		switch (phase_idx)
		{
		case 0:
			if (target != nullptr)
			{
				target->ReleaseDecide();
			}
			ret = SNPhaseResultNext;
			break;

		case 1:
			if (target != nullptr)
			{
				target->ClearButtonSts();
			}
			ret = SNPhaseResultNext;
			break;
		}

	default:
		break;
	}

	return ret;
}
