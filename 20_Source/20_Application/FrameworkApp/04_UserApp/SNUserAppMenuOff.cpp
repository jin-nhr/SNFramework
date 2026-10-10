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


Boolean SNUserAppMenuOff::OnGamePad()
{
	Boolean ret = false;
	Boolean menu = SNVirtualGamePad::Event[SNVirtualGamePadMenu][SNVirtualGamePadEventPush];

	if (menu)
	{
		SNFocus::CallbackPushButton();

		TransCode = SNTransitionCode0;

		ret = true;
	}

	return ret;
}

Boolean SNUserAppMenuOff::OnInternalEvent()
{
	Boolean ret = false;

	if (SNEvent::InternalEvent[SNEventResultDspCreationMenu])
	{
		SNFocus::CallbackPushButton();

		TransCode = SNTransitionCode1;

		ret = true;
	}


	return ret;
}