#pragma once
#include "SNFrameworkInternal.h"
#include "SNMusic.h"
#include "SNMusicResource.h"
#include "SNScene.h"
#include "SNTimerSequencer.h"

enum SNMusicLayer
{
	SNMusicLayerInit,		// –³ŒøƒŒƒCƒ„
	SNMusicLayerLowPri,
	SNMusicLayerLowInt,
	SNMusicLayerMidPri,
	SNMusicLayerMidInt,
	SNMusicLayerHighPri,
	SNMusicLayerHighInt,
	SNMusicLayerMute,
	SNMusicLayerNum
};

class SNMusicPlayer : public SNScene
{
public:
	SNMusicPlayer();
	virtual ~SNMusicPlayer();

	static Void Play(SNMusicLayer layer, SNMusicResID res_id);

	static Void Stop(SNMusicLayer layer);


protected:
	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnCycle();

	virtual SNMusicLayer GetPlayingLayer();

	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

private:
	static SNMusic Player;
	static SNMusicLayer PlayingLayer;
	static SNMusicResID PlayingResID;

	static SNMusicResID ResID[SNMusicLayerNum];
	static Boolean PlayState[SNMusicLayerNum];

	static SNTimerSequencer Seq;
};

