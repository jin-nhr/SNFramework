#include "SNSysAppStartup.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"

// システムアプリ スタートアップ

// コンストラクタ
SNSysAppStartup::SNSysAppStartup()
{
	return;
}

// デストラクタ
SNSysAppStartup::~SNSysAppStartup()
{
	return;
}

// 初期化
Void SNSysAppStartup::OnInitialize()
{
	// テキストラベルの設定
	txtLine1.Initialize();
	txtLine2.Initialize();
	txtLine3.Initialize();

	txtLine1.SetText((String)L"");
	txtLine2.SetText((String)L"");
	txtLine2.SetValue(SNSystemConfig::FrameworkVersion);
	txtLine2.Move(0, 20);
	txtLine3.SetText((String)L"");
	txtLine3.Move(0, 40);

	SetSceneNum(3);
	SetScene(&txtLine1);
	SetScene(&txtLine2);
	SetScene(&txtLine3);


	// 表示用シーケンサ設定
	TimerSeqDisp.Initialize(this, SeqChDisp, 5);
	TimerSeqDisp.SetWait(0, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(1, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(2, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(3, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(4, SeqChDispResultTime);

	// 
	TimerSeqResLoad.Initialize(this, SeqChResLoad, 2);
	TimerSeqResLoad.SetWait(0, 0);
	TimerSeqResLoad.SetWait(1, SeqChResLoadTime);

	return;
}

// 終了処理
Void SNSysAppStartup::OnTerminate()
{
	return;
}

// Entry
Void SNSysAppStartup::OnEntry()
{
	// シーケンサ開始
	TimerSeqDisp.Start();
	TimerSeqResLoad.Start();

	return;
}

// Exit
Void SNSysAppStartup::OnExit()
{
	// この画面の部品はもう使わないのでExitで廃棄
	txtLine1.Terminate();
	txtLine2.Terminate();
	txtLine3.Terminate();

	TimerSeqDisp.Clear();
	TimerSeqResLoad.Clear();

	return;
}

// フレーム処理
Void SNSysAppStartup::OnCycle()
{
	// 周期実行
	TimerSeqResLoad.Step();
	TimerSeqDisp.Step();

	if (!TimerSeqDisp.IsProc())
	{
		if (TimerSeqDisp.IsError())
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
	}
	return;
}



SNPhaseResult SNSysAppStartup::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (ch)
	{
	case SeqChDisp:
		ret = SeqDisp(phase_idx, call_count);
		break;

	case SeqChResLoad:
		ret = SeqResLoad(phase_idx, call_count);
		break;
	}

	return ret;
}

// タイマシーケンサ処理
SNPhaseResult SNSysAppStartup::SeqDisp(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		txtLine1.SetText((String)L"SNFramework System");
		ret = SNPhaseResultNext;
		break;
	case 1:
		txtLine2.SetText((String)L"Version.%08x");
		ret = SNPhaseResultNext;
		break;
	case 2:
		switch (call_count & 0x00000003)
		{
		case 0:
			txtLine3.SetText((String)L"Starting...");
			// カウント3だけで遷移チェックする
			if (!TimerSeqResLoad.IsProc())
			{
				ret = SNPhaseResultNext;
			}
			break;
		case 1:
			txtLine3.SetText((String)L"Starting");
			break;
		case 2:
			txtLine3.SetText((String)L"Starting.");
			break;
		case 3:
			txtLine3.SetText((String)L"Starting..");
			break;
		}
		break;

	case 3:
		// 多重起動チェック
		if (SNSystem::CheckDuplicateInstance())
		{
			txtLine3.SetText((String)L"Starting... Error! [Duplicate Launch]");
		}

		// リソースロードのエラーチェック
		else if (TimerSeqResLoad.IsError())
		{
			txtLine3.SetText((String)L"Starting... Error! [Initialization Failure]");
		}
		else
		{
			txtLine3.SetText((String)L"Starting... Success!");
		}
		ret = SNPhaseResultNext;
		break;

	case 4:
		// 多重起動チェック
		if (SNSystem::CheckDuplicateInstance())
		{
			ret = SNPhaseResultError;
		}
		// リソースロードのエラーチェック
		else if (TimerSeqResLoad.IsError())
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

// リソースロード用シーケンサ処理
SNPhaseResult SNSysAppStartup::SeqResLoad(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		// 起動ロゴ1,2の読み込み
		SNGraphics::LoadSystemResource();
		ret = SNPhaseResultNext;
		break;

	case 1:
		if (SNGraphics::IsSystemResourceLoaded())
		{
			ret = SNPhaseResultNext;
		}
		else if (call_count > SeqChResLoadRetry)
		{
			// エラー時はアクセス権リリース
			SNGraphics::UnloadSystemResource();
			ret = SNPhaseResultError;
		}
		else
		{
			// 完了待ち
		}
		break;
	}

	return ret;
}
