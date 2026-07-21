#pragma once
#include "SNFrameworkInternal.h"
#include "SNSequencer.h"
#include "SNThread.h"


// スレッドシーケンサ
// 順に処理したい手続きを登録し、ThreadをRunするだけであとは勝手に処理してくれる
// 登録した処理は別スレッドで実行される点には注意が必要

class SNThreadSequencer : public SNSequencer, public SNThread
{
public:
	SNThreadSequencer();
    virtual ~SNThreadSequencer();

	// タイマスタート
	virtual Void OnTimerStart(UInt16 wait);
	
	// タイマストップ
	virtual Void OnTimerStop();
	
	// タイマリスタート
	virtual Void OnTimerRestart();
	
	// タイムアウト判定
	virtual Boolean IsTimeout();
    
private:
	// ユーザー実行関数(派生先で実装する)
	virtual Void UserMain();

	UInt32 PhaseStartTime;
	UInt32 Wait;
};
