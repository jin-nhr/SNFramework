#include "SNThreadSequencer.h"
#include "SNWindowsAPI.h"

// コンストラクタ
SNThreadSequencer::SNThreadSequencer()
{
	PhaseStartTime = timeGetTime();
	Wait = -1;

	return;
}

// デストラクタ
SNThreadSequencer::~SNThreadSequencer()
{

	return;
}

// タイマスタート
Void SNThreadSequencer::OnTimerStart(UInt16 wait)
{
	// 開始時間とWaitを記憶
	PhaseStartTime = timeGetTime();
	Wait = wait;

	return;
}

// タイマストップ
Void SNThreadSequencer::OnTimerStop()
{
	// Waitを最大値にしておく
	Wait = -1;

	return;
}

// タイマリスタート
Void SNThreadSequencer::OnTimerRestart()
{
	// 開始時間にWaitを足す
	PhaseStartTime += Wait;

	return;
}

// タイムアウト判定
Boolean SNThreadSequencer::IsTimeout()
{
	UInt32 elapsed_time;

	elapsed_time = timeGetTime() - PhaseStartTime;

	// Wait以上の時間が経過したか？
	return (Wait <= elapsed_time);
}


// スレッドメイン
Void SNThreadSequencer::UserMain()
{
	// シーケンサ開始
	SNSequencer::Start();

	// 処理中はループ
	while (IsProc())
	{
		Step();
		Sleep(1);
	}

	return;
}

