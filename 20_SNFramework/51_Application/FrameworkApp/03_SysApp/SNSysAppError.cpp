#include "SNSysAppError.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"

// システムアプリ スタートアップ

// コンストラクタ
SNSysAppError::SNSysAppError()
{
	return;
}

// デストラクタ
SNSysAppError::~SNSysAppError()
{
	return;
}

// 初期化
Void SNSysAppError::OnInitialize()
{
	// テキストラベルの設定
	txtLine1.Initialize();

	txtLine1.SetText((String)L"Startup failed. Shutting down.");

	SetSceneNum(1);
	SetScene(&txtLine1);

	txtLine1.Centering(true, true);

	// 表示用シーケンサ設定
	TimerSeqDisp.Initialize(this, SeqChDisp, 1);
	TimerSeqDisp.SetWait(0, SeqChDispPhaseTime);

	return;
}

// 終了処理
Void SNSysAppError::OnTerminate()
{
	return;
}

// Entry
Void SNSysAppError::OnEntry()
{
	// シーケンサ開始
	TimerSeqDisp.Start();

	return;
}

// Exit
Void SNSysAppError::OnExit()
{
	// この画面の部品はもう使わないのでExitで廃棄
	txtLine1.Terminate();

	TimerSeqDisp.Clear();

	return;
}

// フレーム処理
Void SNSysAppError::OnCycle()
{
	// 周期実行
	TimerSeqDisp.Step();

	if (!TimerSeqDisp.IsProc())
	{
		// 終了通知
		SNEvent::EventResult[SNEventResultExitApplication] = true;
	}
	return;
}

SNPhaseResult SNSysAppError::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (ch)
	{
	case SeqChDisp:
		ret = SeqDisp(phase_idx, call_count);
		break;
	}

	return ret;
}

// タイマシーケンサ処理
SNPhaseResult SNSysAppError::SeqDisp(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		ret = SNPhaseResultNext;
		break;
	}

	return ret;
}
