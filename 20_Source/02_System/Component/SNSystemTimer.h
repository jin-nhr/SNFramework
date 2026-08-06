#pragma once
#include "SNFrameworkInternal.h"

// システムタイマクラス

class SNSystemTimer
{
public:
	// コンストラクタ
	SNSystemTimer(UInt32 interval_msec);

	// デストラクタ
	~SNSystemTimer();

	// タイマスタート
	Void Start();

	// タイムアウト確認
	Boolean CheckTimeout();

private:
	UInt32 IntervalMsec;		// インターバル(ミリ秒)
	UInt32 StartTimeMSec;	// 開始時間(ミリ秒)
};
