#include "SNWorldOff.h"


Boolean SNWorldOff::OnInternalEvent()
{
	Boolean ret = false;

	// ワールド起動
	if (SNEvent::InternalEvent[SNEventResultEnterWorld])
	{
		TransCode = SNTransitionCode0;

		// アプリ起動は下位レイヤも通すのでfalseにしておく
		ret = false;
	}

	return ret;
}