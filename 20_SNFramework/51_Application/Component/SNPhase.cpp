#include "SNPhase.h"


// Sequencerからのコールバック関数
SNPhaseResult SNPhase::SequenceCallback(SNPhase* phase, Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;
	if (phase != nullptr)
	{
		ret = phase->PhaseStepFunc(ch, phase_idx, call_count);
	}

	return ret;
}


SNPhase::SNPhase()
{
	return;
}

SNPhase::~SNPhase()
{
	return;
}

// クラス内コールバックメソッド
SNPhaseResult SNPhase::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	return SNPhaseResultStay;
}
