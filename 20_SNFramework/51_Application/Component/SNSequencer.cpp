#include "SNSequencer.h"
#include "SNMath.h"
#include "SNPhase.h"

// コンストラクタ
SNSequencer::SNSequencer()
{
    PhaseWait.Clear();
    Phase = nullptr;
    Index = 0;
    CallCount = 0;
    Ch = 0;
    Result = SNSequenceResultIdle;
    return;
}

// デストラクタ
SNSequencer::~SNSequencer()
{
    Clear();
    PhaseWait.Free();

    return;
}

// 初期化
Void SNSequencer::Initialize(SNPhase* phase, Int32 ch, Int32 num)
{
    Int32 loop;
    SNListContainer* cont;

    Clear();
    PhaseWait.Allocate(num);

    for (loop = 0; loop < num; loop++)
    {
        cont = PhaseWait.InsertLast();
        cont->UserData = 0;
    }

    // リストア以降はDirectAccess可
    PhaseWait.Restore();

    Phase = phase;
    Ch = ch;
    Index = 0;
    CallCount = 0;
    Result = SNSequenceResultIdle;

    return;
}

// フェーズ数設定
Void SNSequencer::SetWait(Int32 index, Int32 wait)
{
    SNListContainer* cont;

    UInt32 set_wait = wait;
    set_wait = (UInt32)SNMath::SelectMax(0, set_wait);
    set_wait = (UInt32)SNMath::SelectMin(0xFFFF, set_wait);

    cont = PhaseWait.DirectAccess(index);

    if (cont != nullptr)
    {
        cont->UserData = (Void*)(Int64)set_wait;
    }

    return;
}

// スタート
Void SNSequencer::Start()
{
	if (0 < PhaseWait.GetNum())
    {
        Index = 0;
        CallCount = 0;
		
		OnTimerStart((UInt16)(Int64)PhaseWait.DirectAccess(Index)->UserData);
		
		// 処理中を設定
   		Result = SNSequenceResultProc;
	}
	
    return;
}

// ストップ
Void SNSequencer::Stop()
{
	OnTimerStop();
    Index = 0;
    CallCount = 0;
    Result = SNSequenceResultIdle;
	
    return;
}

// 実行
Void SNSequencer::Step()
{
    SNPhaseResult ret;

    // 登録ありかつ実行中なら処理する
    if ((0 < PhaseWait.GetNum()) &&
        (Result == SNSequenceResultProc))
    {
        // 待ち時間経過
        if (IsTimeout())
        {
            // 呼び出し回数をインクリメント
            CallCount++;

            // コールバックメソッド
            ret = SNPhase::SequenceCallback(Phase, Ch, Index, CallCount);

            switch (ret)
            {
            // Stayのときはタイマ再起動
            case SNPhaseResultStay:
                OnTimerRestart();
                break;

            // Nextのとき
            // 次があるときは次に進める
            // 次がないときは完了する
            case SNPhaseResultNext:
                Index++;
                CallCount = 0;
                if (Index < PhaseWait.GetNum())
                {
                    OnTimerStart((UInt16)(Int64)PhaseWait.DirectAccess(Index)->UserData);
                }
                else
                {
                    OnTimerStop();
                    Result = SNSequenceResultNormal;
                }
                break;

            // Errorのときは途中で終了
            case SNPhaseResultError:
                OnTimerStop();
                Index = 0;
                CallCount = 0;
                Result = SNSequenceResultError;
                break;
            }
        }
    }
	
    return;
}

// クリア
Void SNSequencer::Clear()
{
	OnTimerStop();
	
    PhaseWait.Clear();
    Phase = nullptr;
    Index = 0;
    Ch = 0;
    CallCount = 0;

    Result = SNSequenceResultIdle;
	
    return;
}

// 実行中判定
Boolean SNSequencer::IsProc()
{
    return (Result == SNSequenceResultProc);
}

// 完了？
Boolean SNSequencer::IsEnd()
{
    return (Result == SNSequenceResultNormal);
}

// エラー？
Boolean SNSequencer::IsError()
{
    return (Result == SNSequenceResultError);
}

// タイマスタート
Void SNSequencer::OnTimerStart(UInt16 wait)
{
	return;
}

// タイマストップ
Void SNSequencer::OnTimerStop()
{
	return;
}

// タイマリスタート
Void SNSequencer::OnTimerRestart()
{
	return;
}

// タイムアウト判定
Boolean SNSequencer::IsTimeout()
{
	return false;
}
