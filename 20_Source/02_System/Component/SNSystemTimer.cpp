#include "SNSystemTimer.h"
#include "SNWindowsAPI.h"

// システムタイマクラス

// コンストラクタ
SNSystemTimer::SNSystemTimer(UInt32 interval_msec)
{
    IntervalMsec = interval_msec;
    StartTimeMSec = timeGetTime();
}

// デストラクタ
SNSystemTimer::~SNSystemTimer()
{
}

// タイマスタート
Void SNSystemTimer::Start()
{
    // 開始時間を現在時刻にセット
    StartTimeMSec = timeGetTime();
}

// タイムアウト確認
Boolean SNSystemTimer::CheckTimeout()
{
    // 経過時間をミリ秒単位で算出
    UInt32 elapsed = timeGetTime() - StartTimeMSec;

    // 経過時間がインターバルを超えているか判定
    return (elapsed >= IntervalMsec);
}
