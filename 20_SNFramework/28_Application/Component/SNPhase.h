#pragma once
#include "SNFrameworkInternal.h"
#include "SNSequencer.h"

// フェーズ
class SNPhase
{
public:
	// Sequencerからのコールバック関数
	static SNPhaseResult SequenceCallback(SNPhase* phase, Int32 ch, Int32 phase_idx, Int32 call_count);


public:
	SNPhase();
	virtual ~SNPhase();

	// クラス内コールバックメソッド
	// 派生先でオーバーライドし、各フェーズ処理を実装する
	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);
};
