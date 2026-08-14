#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNTimerSequencer.h"
#include "SNGUIPicture.h"
#include "SNSystemConfig.h"

// システムアプリ ローディング
// 0 - ロード完了
// 1 -
// 2 -
// 3 - エラー
class SNSysAppLoading : public SNLayerController
{
public:
	static constexpr Int32 SeqChDisp = 1;
	static constexpr Int32 SeqChResLoad = 2;

	static constexpr Int32 SeqChDispFadeTime = SNSystemConfig::FadeTime + 100;
	static constexpr Int32 SeqChDispPhaseTime = 3000;

	static constexpr Int32 SeqChResLoadTime = 3000;
	static constexpr Int32 SeqChResLoadRetry = 3;

public:
	// コンストラクタ
	SNSysAppLoading();

	// デストラクタ
	virtual ~SNSysAppLoading();

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
	SNGUIPicture pctLogo;
	SNTimerSequencer	  TimerSeqDisp;		// 表示用タイマシーケンサ
	SNTimerSequencer  TimerSeqResLoad;	// リソースロード用シーケンサ

};

