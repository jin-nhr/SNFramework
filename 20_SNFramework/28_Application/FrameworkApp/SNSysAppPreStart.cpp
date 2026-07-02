#include "SNSysAppPreStart.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"

// デバッグアプリ OFF

// コンストラクタ
SNSysAppPreStart::SNSysAppPreStart()
{
	return;
}

// デストラクタ
SNSysAppPreStart::~SNSysAppPreStart()
{
	return;
}

// 初期化
Void SNSysAppPreStart::OnInitialize()
{
	LblLine1.Initialize();
	LblLine2.Initialize();
	LblLine3.Initialize();

	LblLine1.SetText((String)L"");
	LblLine2.SetText((String)L"");
	LblLine2.SetValue(SNSystemConfig::FrameworkVersion);
	LblLine2.Move(0, 20);
	LblLine3.SetText((String)L"");
	LblLine3.Move(0, 40);

	SetSceneNum(3);
	SetScene(&LblLine1);
	SetScene(&LblLine2);
	SetScene(&LblLine3);

	TimerSeq.Initialize(this, SequencerChTimer, 5);
	TimerSeq.SetWait(0, SeqPhaseTimeProc);
	TimerSeq.SetWait(1, SeqPhaseTimeProc);
	TimerSeq.SetWait(2, SeqPhaseTimeProc);
	TimerSeq.SetWait(3, SeqPhaseTimeProc);
	TimerSeq.SetWait(4, SeqPhaseTimeResult);

	ThreadSeq.Initialize(this, SequencerChThread, 1);
	ThreadSeq.SetWait(0, SeqPhaseTimeProc);


	return;
}

// 終了処理
Void SNSysAppPreStart::OnTerminate()
{
	return;
}

// Entry
Void SNSysAppPreStart::OnEntry()
{
	// シーケンサ開始
	ThreadSeq.Run();
	TimerSeq.Start();

	return;
}

// Exit
Void SNSysAppPreStart::OnExit()
{
	// この画面の部品はもう使わないのでExitで廃棄
	LblLine1.Terminate();
	LblLine2.Terminate();
	LblLine3.Terminate();

	ThreadSeq.Clear();
	TimerSeq.Clear();

	return;
}

// フレーム処理
Void SNSysAppPreStart::OnCycle()
{
	// タイマシーケンサ実行中
	if (TimerSeq.IsProc())
	{
		// 周期実行
		TimerSeq.Step();
	}
	else if (TimerSeq.IsError())
	{
		// エラー時は終了
		SNEvent::EventResult[SNEventResultExitApplication] = true;
	}
	else
	{
		// 完了時
		TransCode = SNTransitionCode0;
		SNEvent::EventResult[SNEventResultSysAppStartup] = true;
	}

	return;
}

// 描画処理
Void SNSysAppPreStart::OnDraw(SNSurface* surface)
{
	// この状態ではシステムリソースの準備ができていないため
	// 直接サーフェスをクリアする
	surface->Clear();

	return;
}

SNPhaseResult SNSysAppPreStart::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (ch)
	{
	// TimerSeq
	case SequencerChTimer:
		ret = TimerSeqPhase(phase_idx, call_count);
		break;

	// ThreadSeq
	case SequencerChThread:
		ret = ThreadSeqPhase(phase_idx, call_count);
		break;
	}

	return ret;
}



// タイマシーケンサ処理
SNPhaseResult SNSysAppPreStart::TimerSeqPhase(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		LblLine1.SetText((String)L"SNFramework System");
		ret = SNPhaseResultNext;
		break;
	case 1:
		LblLine2.SetText((String)L"Version.%08x");
		ret = SNPhaseResultNext;
		break;
	case 2:
		switch (call_count & 0x00000003)
		{
		case 0:
			LblLine3.SetText((String)L"Starting...");
			// カウント3だけで遷移チェックする
			if (!ThreadSeq.IsProc())
			{
				ret = SNPhaseResultNext;
			}
			break;
		case 1:
			LblLine3.SetText((String)L"Starting");
			break;
		case 2:
			LblLine3.SetText((String)L"Starting.");
			break;
		case 3:
			LblLine3.SetText((String)L"Starting..");
			break;
		}
		break;

	case 3:
		// 多重起動チェック
		if (SNSystem::CheckDuplicateInstance())
		{
			LblLine3.SetText((String)L"Starting... Error! [Duplicate Launch]");
		}

		// ThreadSeqのエラーチェック
		else if (ThreadSeq.IsError())
		{
			LblLine3.SetText((String)L"Starting... Error! [Initialization Failure]");
		}
		else
		{
			LblLine3.SetText((String)L"Starting... Success!");
		}
		ret = SNPhaseResultNext;
		break;

	case 4:
		// 多重起動チェック
		if (SNSystem::CheckDuplicateInstance())
		{
			ret = SNPhaseResultError;
		}
		// ThreadSeqのエラーチェック
		else if (ThreadSeq.IsError())
		{
			ret = SNPhaseResultError;
		}
		else
		{
			ret = SNPhaseResultNext;
		}
		break;
	}

	return ret;
}

// スレッドシーケンサ処理
SNPhaseResult SNSysAppPreStart::ThreadSeqPhase(Int32 phase_idx, Int32 call_count)
{
	SNGraphics::LoadSystemResource();
	return SNPhaseResultNext;
}
