#include "SNWorld.h"
#include "SNMusicPlayer.h"


Void SNWorld::OnEntry()
{
	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResEnterWorld);
	return;
}

Void SNWorld::OnExit()
{
	SNMusicPlayer::Stop(SNMusicLayerLowPri);

	return;
}

