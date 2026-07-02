#pragma once
#include "SNFrameworkInternal.h"
#include "SNSequencer.h"
#include "SNSoftTimer.h"


// タイマシーケンサ
// アプリのフレーム処理内で順に処理したい手続きの実行制御

class SNTimerSequencer : public SNSequencer
{
public:
    SNTimerSequencer();
    virtual ~SNTimerSequencer();

	// タイマスタート
	virtual Void OnTimerStart(UInt16 wait);
	
	// タイマストップ
	virtual Void OnTimerStop();
	
	// タイマリスタート
	virtual Void OnTimerRestart();
	
	// タイムアウト判定
	virtual Boolean IsTimeout();
    
private:
    SNSoftTimer Timer;  // タイマ
};
