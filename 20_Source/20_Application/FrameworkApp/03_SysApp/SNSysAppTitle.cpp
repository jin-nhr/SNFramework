#include "SNSysAppTitle.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFocus.h"
#include "SNVirtualGamePad.h"
#include "SNFixedString.h"


SNRect SNSysAppTitle::GUIButtonRect[GUIButtonNum] =
{
	{32, 16, 160, 32},
	{32, 48, 160, 32},
	{32, 80, 160, 32},
	{32, 112,160, 32},
};

String SNSysAppTitle::GUIButtonString[GUIButtonNum] =
{
	SNFixedString::TitleMenuNew,
	SNFixedString::TitleMenuCont,
	SNFixedString::TitleMenuConfig,
	SNFixedString::TitleMenuExit,
};



// コンストラクタ
SNSysAppTitle::SNSysAppTitle()
{
	return;
}

// デストラクタ
SNSysAppTitle::~SNSysAppTitle()
{
	return;
}


Void SNSysAppTitle::OnInitialize()
{
	Int32 cnt;

	GUIWin.Initialize();
	GUIWin.SetRect(0, 336, 224,	160);
	GUIWin.Centering(false, true);
	GUIWin.SetSceneNum(GUIButtonNum);

	FocusGp.StartRegister(SNFocusLayerSysApp, GUIButtonNum, true, false);

	for (cnt = 0; cnt < GUIButtonNum; cnt++)
	{
		GUIButton[cnt].Initialize();
		GUIButton[cnt].SetRect(
			GUIButtonRect[cnt].PointX,
			GUIButtonRect[cnt].PointY,
			GUIButtonRect[cnt].Width,
			GUIButtonRect[cnt].Height);
		GUIButton[cnt].Caption.SetText(GUIButtonString[cnt]);
		GUIButton[cnt].Caption.Centering(true, true);
		GUIButton[cnt].NoFrame = true;
		GUIWin.SetScene(&GUIButton[cnt]);
		FocusGp.RegisterButton(&GUIButton[cnt]);
	}

	FocusGp.EndRegister();

	SetSceneNum(1);
	SetScene(&GUIWin);

	Seq.Initialize(this, 0, 1);
	Seq.SetWait(0, 0);

	return;
}

Void SNSysAppTitle::OnTerminate()
{
	
	return;
}


// Entry
Void SNSysAppTitle::OnEntry()
{
	SNMute::SetMute(false, true);

	FocusGp.Entry();

	return;
}

Void SNSysAppTitle::OnExit()
{
	FocusGp.Exit();

	return;
}


Boolean SNSysAppTitle::OnGamePad1()
{
	Boolean ret = false;

	return ret;
}

Void SNSysAppTitle::OnCycle()
{
	if (Seq.IsProc())
	{
		Seq.Step();
	}
	else
	{
		// New
		if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonNew]))
		{
			GUIWin.Visible = false;

			SNMute::SetMute(true, true);

			Seq.Start();
		}

		// Continue
		else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonLoad]))
		{
			// ★Load画面へ
		}

		// Config
		else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonConfig]))
		{
			TransCode = SNTransitionCode2;
		}

		// Exit
		else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonExit]))
		{
			TransCode = SNTransitionCode3;
		}
	}
	return;
}

// イベント通知
Boolean SNSysAppTitle::OnNotifyEvent()
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

SNPhaseResult SNSysAppTitle::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (ch)
	{
	case 0:
		switch (phase_idx)
		{
		case 0:
			// ミュート待ち
			if (SNMute::GetNowSts())
			{
				// ワールド起動イベント発行
				SNEvent::EventResult[SNEventResultEnterWorld] = true;
				TransCode = SNTransitionCode0;
				ret = SNPhaseResultNext;
			}
			break;
		}
		break;
	}
	return ret;
}
