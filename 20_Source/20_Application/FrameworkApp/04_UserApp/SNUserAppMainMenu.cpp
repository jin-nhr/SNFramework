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
	{16, 256, 160, 32},
	{16, 304, 160, 32},
};

String SNUserAppMainMenu::ButtonCaption[SNMainMenuButtonNum] =
{
	SNFixedString::MenuCreateMap,
	SNFixedString::MenuGroundPreset,
	SNFixedString::MenuSetStaticObject,
	SNFixedString::MenuSetActiveObject,
	SNFixedString::MenuTestPlay,
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

	SetRect(32, 32, 192, 352);

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
	if (FocusGp.JudgeActDecide(&Button[SNMainMenuButtonMap]))
	{
		SNEvent::EventResult[SNEventResultMenuMapCreate] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNMainMenuButtonTestPlay]))
	{
		SNEvent::EventResult[SNEventResultMenuTestPlay] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNMainMenuButtonConfig]))
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
