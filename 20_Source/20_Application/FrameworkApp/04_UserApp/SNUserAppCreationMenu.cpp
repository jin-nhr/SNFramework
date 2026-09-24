#include "SNUserAppCreationMenu.h"
#include "SNFixedString.h"
#include "SNFocus.h"

SNRect SNUserAppCreationMenu::ButtonRect[SNCreationMenuButtonNum] =
{
	{16, 16, 160, 32},
	{16, 64, 160, 32},
	{16, 112, 160, 32},
	{16, 160, 160, 32},
	{16, 208, 160, 32},
};

String SNUserAppCreationMenu::ButtonCaption[SNCreationMenuButtonNum] =
{
	SNFixedString::MenuCopy,
	SNFixedString::MenuCut,
	SNFixedString::MenuRotateL,
	SNFixedString::MenuRotateR,
	SNFixedString::MenuDeselect,
};


SNUserAppCreationMenu::SNUserAppCreationMenu()
{
	return;
}

SNUserAppCreationMenu::~SNUserAppCreationMenu()
{
	return;
}

Void SNUserAppCreationMenu::OnInitialize()
{
	Int32 cnt;

	SetRect(SNSystemConfig::ScreenWidth / 2, SNSystemConfig::ScreenHeight / 2, 192, 256);

	SetSceneNum(SNCreationMenuButtonNum);
	FocusGp.StartRegister(SNFocusLayerUserApp, SNCreationMenuButtonNum, true, false);
	for (cnt = 0; cnt < SNCreationMenuButtonNum; cnt++)
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

Void SNUserAppCreationMenu::OnEntry()
{
	FocusGp.Entry();

	return;
}

Void SNUserAppCreationMenu::OnExit()
{
	FocusGp.Exit();

	return;
}

Void SNUserAppCreationMenu::OnCycle()
{
	if (FocusGp.JudgeActDecide(&Button[SNCreationMenuButtonCopy]))
	{
		SNEvent::EventResult[SNEventResultMenuCopy] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNCreationMenuButtonCut]))
	{
		SNEvent::EventResult[SNEventResultMenuCut] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNCreationMenuButtonRotateL]))
	{
		SNEvent::EventResult[SNEventResultMenuRotateL] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNCreationMenuButtonRotateR]))
	{
		SNEvent::EventResult[SNEventResultMenuRotateR] = true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	else if (FocusGp.JudgeActDecide(&Button[SNCreationMenuButtonDeselect]))
	{
		SNEvent::EventResult[SNEventResultMenuDeselect]= true;
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	if (FocusGp.JudgeActCancel())
	{
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	return;
}
