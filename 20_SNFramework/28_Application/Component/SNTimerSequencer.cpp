#include "SNTimerSequencer.h"


// コンストラクタ
SNTimerSequencer::SNTimerSequencer()
{
    Timer.Initialize();
	
	return;
}

// デストラクタ
SNTimerSequencer::~SNTimerSequencer()
{

	return;
}

// タイマスタート
Void SNTimerSequencer::OnTimerStart(UInt16 wait)
{
	Timer.Start(wait);
	
	return;
}

// タイマストップ
Void SNTimerSequencer::OnTimerStop()
{
	Timer.Stop();	

	return;
}

// タイマリスタート
Void SNTimerSequencer::OnTimerRestart()
{
	Timer.Restart();
	
	return;
}

// タイムアウト判定
Boolean SNTimerSequencer::IsTimeout()
{
	return Timer.IsTimeout();
}
