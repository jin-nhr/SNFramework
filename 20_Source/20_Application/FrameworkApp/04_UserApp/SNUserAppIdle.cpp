#include "SNUserAppIdle.h"
#include "SNMusicPlayer.h"

SNUserAppIdle::SNUserAppIdle()
{
	return;
}

SNUserAppIdle::~SNUserAppIdle()
{
	return;
}

Void SNUserAppIdle::OnEntry()
{
	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResEnterWorld);
	return;
}

