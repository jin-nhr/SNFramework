#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUITextLabelEx.h"
#include "SNThreadSequencer.h"
#include "SNTimerSequencer.h"


// システムアプリ 起動準備
// 処理完了時 - SNTransitionCode0
// エラー時   - 直接アプリ終了通知発行
class SNSysAppPreStart : public virtual SNLayerController
{
public:
	static constexpr Int32 SequencerChTimer = 1;
	static constexpr Int32 SequencerChThread = 2;

	static constexpr Int32 SeqPhaseTimeProc = 500;
	static constexpr Int32 SeqPhaseTimeResult = 1000;

public:
	// コンストラクタ
	SNSysAppPreStart();

	// デストラクタ
	virtual ~SNSysAppPreStart();

protected:

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// フレーム処理
	virtual Void OnCycle();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	// シーケンサ用コールバック
	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

	// タイマシーケンサ処理
	virtual SNPhaseResult TimerSeqPhase(Int32 phase_idx, Int32 call_count);

	// スレッドシーケンサ処理
	virtual SNPhaseResult ThreadSeqPhase(Int32 phase_idx, Int32 call_count);

private:
	SNGUITextLabelEx	LblLine1;
	SNGUITextLabelEx	LblLine2;
	SNGUITextLabelEx	LblLine3;
	SNThreadSequencer	ThreadSeq;		// スレッドシーケンサ
	SNTimerSequencer	TimerSeq;		// タイマシーケンサ
};

