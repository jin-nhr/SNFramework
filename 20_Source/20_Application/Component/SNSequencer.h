#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"

// シーケンサクラス


// 参照クラス
class SNPhase;

// フェーズ処理結果
enum SNPhaseResult
{
    SNPhaseResultStay,
    SNPhaseResultNext,
    SNPhaseResultError
};

// シーケンス処理結果
enum SNSequenceResult
{
    SNSequenceResultIdle,
    SNSequenceResultProc,
    SNSequenceResultNormal,
    SNSequenceResultError,
};

class SNSequencer
{
public:
    SNSequencer();
    virtual ~SNSequencer();
	
    // 初期化
    virtual Void Initialize(SNPhase* phase, Int32 ch, Int32 num);

    // ウェイト設定
    virtual Void SetWait(Int32 index, Int32 wait);

    // スタート
    virtual Void Start();

    // ストップ
    virtual Void Stop();

    // 実行
    virtual Void Step();

    // クリア
    virtual Void Clear();

	// 実行中判定
	virtual Boolean IsProc();
	
	// 完了判定
    virtual Boolean IsEnd();

	// エラー判定
    virtual Boolean IsError();
	
	// タイマスタート
	virtual Void OnTimerStart(UInt16 wait);
	
	// タイマストップ
	virtual Void OnTimerStop();
	
	// タイマリスタート
	virtual Void OnTimerRestart();
	
	// タイムアウト判定
	virtual Boolean IsTimeout();

protected:
    SNPhase* Phase;     // フェーズIF
    Int32 Ch;           // チャンネル
    Int32 Index;        // 現在のフェーズ番号
    Int32 CallCount;    // 現フェーズの呼び出し回数
    SNList PhaseWait;   // ウェイトリスト
    volatile SNSequenceResult Result;    // 処理結果
};
