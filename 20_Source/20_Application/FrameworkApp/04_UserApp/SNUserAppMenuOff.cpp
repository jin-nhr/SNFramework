#include "SNUserAppMenuOff.h"
#include "SNVirtualGamePad.h"
#include "SNFocus.h"

SNUserAppMenuOff::SNUserAppMenuOff()
{
	return;
}

SNUserAppMenuOff::~SNUserAppMenuOff()
{
	return;
}


Boolean SNUserAppMenuOff::OnGamePad1()
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
