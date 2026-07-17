#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUISystemTextEx.h"
#include "SNTimerSequencer.h"

// システムアプリ スタートアップ
// 処理完了時 - SNTransitionCode0
// エラー時   - 直接アプリ終了通知発行
class SNSysAppStartup : public virtual SNLayerController
{
public:
	static constexpr Int32 SeqChDisp = 1;
	static constexpr Int32 SeqChResLoad = 2;

	static constexpr Int32 SeqChDispPhaseTime = 1000;
	static constexpr Int32 SeqChDispResultTime = 3000;

	static constexpr Int32 SeqChResLoadTime = 3000;
	static constexpr Int32 SeqChResLoadRetry = 3;

public:
	// コンストラクタ
	SNSysAppStartup();

	// デストラクタ
	virtual ~SNSysAppStartup();

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

	// シーケンサ用コールバック
	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

	// 表示用タイマシーケンサ処理
	virtual SNPhaseResult SeqDisp(Int32 phase_idx, Int32 call_count);

	// リソースロード用シーケンサ処理
	virtual SNPhaseResult SeqResLoad(Int32 phase_idx, Int32 call_count);

private:
	SNGUISystemTextEx txtLine1;
	SNGUISystemTextEx txtLine2;
	SNGUISystemTextEx txtLine3;
	SNTimerSequencer	  TimerSeqDisp;		// 表示用タイマシーケンサ
	SNTimerSequencer  TimerSeqResLoad;	// リソースロード用シーケンサ
};

