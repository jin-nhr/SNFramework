#include "SNUserAppOff.h"
#include "SNEvent.h"

SNUserAppOff::SNUserAppOff()
{
	return;
}

SNUserAppOff::~SNUserAppOff()
{
	return;
}


Boolean SNUserAppOff::OnInternalEvent()
{
	Boolean ret = false;

	// 起動通知あり
	if (SNEvent::InternalEvent[SNEventResultAppStartup])
	{
		TransCode = SNTransitionCode0;

		// アプリ起動は下位レイヤも通すのでfalseにしておく
		ret = false;
	}

	return ret;
}
