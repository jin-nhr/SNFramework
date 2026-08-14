#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNMute.h"
#include "SNFocus.h"
#include "SNMusicPlayer.h"


class SNOverlapApp : public SNLayerController
{
public:
	SNOverlapApp();
	virtual ~SNOverlapApp();


	SNMute MuteApp;
	SNFocus Focus;
	SNMusicPlayer MusicPlayer;
};

