#include "SNUserAppTitle.h"
#include "SNGraphicsResManager.h"
#include "SNMusicPlayer.h"

SNUserAppTitle::SNUserAppTitle()
{
	return;
}

SNUserAppTitle::~SNUserAppTitle()
{
	return;
}

Void SNUserAppTitle::OnInitialize()
{
	return;
}

Void SNUserAppTitle::OnTerminate()
{
	return;
}

Void SNUserAppTitle::OnEntry()
{
	SNGraphicsResManager::AccessGet(SNGraphicsResAppTitle);

	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResTitle);

	return;
}

Void SNUserAppTitle::OnExit()
{
	SNGraphicsResManager::AccessRelease(SNGraphicsResAppTitle);
	SNMusicPlayer::Stop(SNMusicLayerLowPri);

	return;
}

Boolean SNUserAppTitle::OnInternalEvent()
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


Void SNUserAppTitle::OnPreDraw()
{
	SetBitmap(SNGraphicsResManager::GetResource(SNGraphicsResAppTitle));

	return;
}

