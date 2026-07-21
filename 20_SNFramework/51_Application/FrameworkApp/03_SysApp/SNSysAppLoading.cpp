#include "SNSysAppLoading.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNMute.h"

// システムアプリ スタートアップ

// コンストラクタ
SNSysAppLoading::SNSysAppLoading()
{
	return;
}

// デストラクタ
SNSysAppLoading::~SNSysAppLoading()
{
	return;
}

// 初期化
Void SNSysAppLoading::OnInitialize()
{
	// ピクチャー初期化
	pctLogo.Initialize();
	pctLogo.Visible = false;		// ミュート前に一瞬ちらつく問題回避

	SetSceneNum(1);
	SetScene(&pctLogo);



	// 表示用シーケンサ設定
	TimerSeqDisp.Initialize(this, SeqChDisp, 7);
	TimerSeqDisp.SetWait(0, 0);
	TimerSeqDisp.SetWait(1, SeqChDispFadeTime);
	TimerSeqDisp.SetWait(2, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(3, SeqChDispFadeTime);
	TimerSeqDisp.SetWait(4, SeqChDispFadeTime);
	TimerSeqDisp.SetWait(5, SeqChDispPhaseTime);
	TimerSeqDisp.SetWait(6, SeqChDispFadeTime);
	// リソースロード
	TimerSeqResLoad.Initialize(this, SeqChResLoad, 2);
	TimerSeqResLoad.SetWait(0, 0);
	TimerSeqResLoad.SetWait(1, SeqChResLoadTime);

	return;
}

// 終了処理
Void SNSysAppLoading::OnTerminate()
{
	return;
}

// Entry
Void SNSysAppLoading::OnEntry()
{
	// ピクチャ設定
	pctLogo.SetBitmap(SNGraphicsResManager::GetResource(SNGraphicsResStartLogo1));

	// ミュート設定
	SNMute::SetMute(true, false);

	// シーケンサ開始
	TimerSeqDisp.Start();
	TimerSeqResLoad.Start();

	return;
}

// Exit
Void SNSysAppLoading::OnExit()
{
	// この画面の部品はもう使わないのでExitで廃棄
	pctLogo.Terminate();

	// ロゴ画像はもう使わないのでアンロードする
	// 連続定義が絶対ではないので1個ずつ処理
	SNGraphics::UnloadResource(SNGraphicsResStartLogo1, SNGraphicsResStartLogo1);
	SNGraphics::UnloadResource(SNGraphicsResStartLogo2, SNGraphicsResStartLogo2);

	SNMute::SetMute(false, true);

	return;
}

// フレーム処理
Void SNSysAppLoading::OnCycle()
{
	// 周期実行
	TimerSeqResLoad.Step();
	TimerSeqDisp.Step();

	if (!TimerSeqDisp.IsProc())
	{
		if (TimerSeqDisp.IsError())
		{
			// エラー時は遷移先を変える
			TransCode = SNTransitionCode3;
		}
		else
		{
			// 完了時
			TransCode = SNTransitionCode0;
		}
	}
	return;
}



SNPhaseResult SNSysAppLoading::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
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
SNPhaseResult SNSysAppLoading::SeqDisp(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		ret = SNPhaseResultNext;

		break;
	case 1:
		// フェードイン設定
		SNMute::SetMute(false, true);
		pctLogo.Visible = true;
		ret = SNPhaseResultNext;
		break;
	case 2:
		// フェードアウト設定
		SNMute::SetMute(true, true);
		ret = SNPhaseResultNext;
		break;
	case 3:
		// ロゴ切替
		pctLogo.SetBitmap(SNGraphicsResManager::GetResource(SNGraphicsResStartLogo2));
		// フェードイン設定
		SNMute::SetMute(false, true);
		ret = SNPhaseResultNext;
		break;
	case 4:
		ret = SNPhaseResultNext;
		break;
	case 5:
		// リソースロード完了チェック
		if (!TimerSeqResLoad.IsProc())
		{
			// フェードアウト設定
			SNMute::SetMute(true, true);
			ret = SNPhaseResultNext;
		}
		break;
	case 6:
		// リソースのロード結果を確認
		if (TimerSeqResLoad.IsError())
		{
			ret = SNPhaseResultError;
		}
		else
		{
			ret = SNPhaseResultNext;
		}
	}

	return ret;
}

// リソースロード用シーケンサ処理
SNPhaseResult SNSysAppLoading::SeqResLoad(Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;

	switch (phase_idx)
	{
	case 0:
		// 起動ロゴ1,2の読み込み
		SNGraphics::LoadAppResource();
		ret = SNPhaseResultNext;
		break;

	case 1:
		if (SNGraphics::IsAppResourceLoaded())
		{
			ret = SNPhaseResultNext;
		}
		else if (call_count > SeqChResLoadRetry)
		{
			// エラー時はアクセス権リリース
			SNGraphics::UnloadAppResource();
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
