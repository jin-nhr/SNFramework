#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUISystemTextEx.h"
#include "SNTimerSequencer.h"

// システムアプリ エラー画面
// 0 -
// 1 -
// 2 -
// 3 -
// 時間経過で直接アプリ終了する

class SNSysAppError : public SNLayerController
{
public:
	static constexpr Int32 SeqChDisp = 1;

	static constexpr Int32 SeqChDispPhaseTime = 3000;

public:
	// コンストラクタ
	SNSysAppError();

	// デストラクタ
	virtual ~SNSysAppError();

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


private:
	SNGUISystemText   txtLine1;
	SNTimerSequencer	  TimerSeqDisp;		// 表示用タイマシーケンサ
};

