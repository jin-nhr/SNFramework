#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNList.h"
#include "SNTimerSequencer.h"

class SNFocusGroup;


class SNFocus : public SNScene
{
public:
	// システムアプリの登録/解除
	static Void RegisterSysApp(SNFocusGroup* sysapp);
	static Void ReleaseSysApp(SNFocusGroup* sysapp);

	// ユーザーアプリの登録/解除
	static Void RegisterUserApp(SNFocusGroup* userapp);
	static Void ReleaseUserApp(SNFocusGroup* userapp);

	static SNFocusGroup* SysAppGroup;
	static SNFocusGroup* UserAppGroup;

public:
	SNFocus();
	virtual ~SNFocus();

	virtual SNFocusGroup* GetTarget();

	virtual Boolean OnPointingDevice();

	virtual Boolean OnGamePad1();

	virtual Void OnCycle();

	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

	SNTimerSequencer TimerSeq;
};

