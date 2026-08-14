#include "SNSysAppPreClose.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFixedString.h"

// コンストラクタ
SNSysAppPreClose::SNSysAppPreClose()
{
	Size.Width = 256;
	Size.Height = 128;

	SetSceneNum(3);
	SetScene(&ButtonYes);
	SetScene(&ButtonNo);
	SetScene(&TxtMessage);

	FocusGp.StartRegister(SNFocusLayerSysApp, 2, false, false);
	FocusGp.RegisterButton(&ButtonYes);
	FocusGp.RegisterButton(&ButtonNo);
	FocusGp.EndRegister();

	Centering(true, true);

	TxtMessage.Move(0, 24);

	ButtonYes.Resize(80, 32);
	ButtonYes.Move(24, 60);

	ButtonNo.Resize(80, 32);
	ButtonNo.Move(152, 60);

	return;
}

// デストラクタ
SNSysAppPreClose::~SNSysAppPreClose()
{
	return;
}

// Entry
Void SNSysAppPreClose::OnEntry()
{
	FocusGp.Entry();

	// テキストはInitializeでは設定できない
	TxtMessage.SetText(SNFixedString::PreCloseMsg);
	TxtMessage.Centering(false, true);

	ButtonYes.Caption.SetText(SNFixedString::PreCloseYes);
	ButtonYes.Caption.Centering(true, true);

	ButtonNo.Caption.SetText(SNFixedString::PreCloseNo);
	ButtonNo.Caption.Centering(true, true);

	return;
}

Void SNSysAppPreClose::OnExit()
{
	FocusGp.Exit();

	return;
}


// フレーム処理
Void SNSysAppPreClose::OnCycle()
{
	if (FocusGp.JudgeActDecide(&ButtonYes))
	{
		// アプリ終了
		SNEvent::EventResult[SNEventResultExitApplication] = true;
	}

	else if (FocusGp.JudgeActDecide(&ButtonNo))
	{
		TransCode = SNTransitionCode0;
	}

	else if (FocusGp.JudgeActCancel())
	{
		TransCode = SNTransitionCode0;
		SNFocus::CallbackPushButton();
	}

	// 最後に状態クリアしておく
	FocusGp.ClearButtonSts();

	return;
}

