#include "SNFPSTimer.h"
#include "../System/SNWindowsAPI.h"

// FPSタイマクラス

#define SNFPSTIMER_SLEEP_THRESHOLD_TIME	(2000)


// コンストラクタ
SNFPSTimer::SNFPSTimer()
{
	// 変数初期化
	IntervalMicroSecond = 0;
	NextTimeMicroSecond = 0;
	StartTimeMilliSecond = timeGetTime();

	return;
}

// デストラクタ
SNFPSTimer::~SNFPSTimer()
{

	return;
}

// FPS設定
Void SNFPSTimer::SetPFS(UInt32 fps)
{
	// FPSからマイクロ秒単位のインターバルを計算
	IntervalMicroSecond = 1000000 / fps;

	return;
}

// タイマスタート
Void SNFPSTimer::Start()
{
	// タイムアウト時間にインターバルをセット
	NextTimeMicroSecond = IntervalMicroSecond;

	// 開始時間を取得
	StartTimeMilliSecond = timeGetTime();

	return;
}

// タイマリスタート
Void SNFPSTimer::Restart()
{
	// 現在時間を取得
	UInt32 now_time = timeGetTime();

	// 次のタイムアウト時間を計算
	// インターバルから前回タイムアウト時間を超過した分を減算
	NextTimeMicroSecond = IntervalMicroSecond - ((now_time - StartTimeMilliSecond) * 1000 - NextTimeMicroSecond);

	// 開始時間を現在時間で更新
	StartTimeMilliSecond = now_time;

	return;
}

// タイムアウト確認
Boolean SNFPSTimer::CheckTimeout()
{
	// 経過時間をマイクロ秒単位で算出
	Int64 elapsed_time = (timeGetTime() - StartTimeMilliSecond) * 1000;

	// 経過時間がタイムアウト時間を超えているか判定
	return (NextTimeMicroSecond < elapsed_time);
}

// Sleepする
Void SNFPSTimer::Sleep()
{
	// タイムアウトまでの残り時間計算
	Int64 remaining_time = NextTimeMicroSecond - ((timeGetTime() - StartTimeMilliSecond) * 1000);

	// 残り2ms以上ある？
	if (remaining_time >= SNFPSTIMER_SLEEP_THRESHOLD_TIME)
	{
		// 1msのSleepを行う
		::Sleep(1);
	}
	else
	{
		// 高優先度のスレッドがあればCPUを明け渡す
		::Sleep(0);
	}

	return;
}
