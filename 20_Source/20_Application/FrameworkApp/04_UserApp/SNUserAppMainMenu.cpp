#include "SNUserAppMainMenu.h"
#include "SNFixedString.h"
#include "SNFocus.h"

SNRect SNUserAppMainMenu::ButtonRect[SNMainMenuButtonNum] =
{
	{16, 16, 160, 32},
	{16, 64, 160, 32},
	{16, 112, 160, 32},
	{16, 160, 160, 32},
	{16, 208, 160, 32},
};

String SNUserAppMainMenu::ButtonCaption[SNMainMenuButtonNum] =
{
	(String)L"メニュー1",
	(String)L"メニュー2",
	(String)L"メニュー3",
	SNFixedString::TitleMenuConfig,
	SNFixedString::TitleMenuExit,
};


SNUserAppMainMenu::SNUserAppMainMenu()
{
	return;
}

SNUserAppMainMenu::~SNUserAppMainMenu()
{
	return;
}

Void SNUserAppMainMenu::OnInitialize()
{
	Int32 cnt;

	SetRect(32, 32, 192, 256);

	SetSceneNum(SNMainMenuButtonNum);
	FocusGp.StartRegister(SNFocusLayerUserApp, SNMainMenuButtonNum, true, false);
	for (cnt = 0; cnt < SNMainMenuButtonNum; cnt++)
	{
		SetScene(&Button[cnt]);
		FocusGp.RegisterButton(&Button[cnt]);
		Button[cnt].SetRect(
			ButtonRect[cnt].PointX,
			ButtonRect[cnt].PointY,
			ButtonRect[cnt].Width,
			ButtonRect[cnt].Height);
		Button[cnt].NoFrame = true;
		Button[cnt].Caption.SetText(ButtonCaption[cnt]);
		Button[cnt].Caption.Centering(true, true);
	}
	FocusGp.EndRegister();

	return;
}

Void SNUserAppMainMenu::OnEntry()
{
	FocusGp.Entry();

	return;
}

Void SNUserAppMainMenu::OnExit()
{
	FocusGp.Exit();

	return;
}

Void SNUserAppMainMenu::OnCycle()
{
	if (FocusGp.JudgeActDecide(&Button[SNMainMenuButtonConfig]))
	{
		SNEvent::EventResult[SNEventResultDspConfig] = true;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNMainMenuButtonExit]))
	{
		SNEvent::EventResult[SNEventResultDspPreClose]= true;
	}

	if (FocusGp.JudgeActCancel())
	{
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	return;
}
