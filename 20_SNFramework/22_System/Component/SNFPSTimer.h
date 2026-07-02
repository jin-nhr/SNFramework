#pragma once
#include "SNFrameworkInternal.h"

// FPSタイマクラス
// FPSに従ってフレームタイミングを測る
// また次フレームまで待機する機能を持つ
// FPSに見たない場合にはスキップ判断も行う

class SNFPSTimer
{
public:
	static constexpr UInt32 FrameSkip = 3;				// 最大フレームスキップ
	static constexpr UInt32 SleepTimeThreshold = 5000;	// Sleep時間閾値(ms)

public:
	// コンストラクタ
	SNFPSTimer();

	// デストラクタ
	~SNFPSTimer();

	// タイマスタート
	Void Start();

	// タイマリスタート
	Void Restart();

	// タイムアウト確認
	Boolean CheckTimeout();

	// Sleepする
	Void Sleep();

	// スキップフラグ取得
	Boolean GetSkipFlag();

private:
	Int64 IntervalMicroSecond;		// インターバル(マイクロ秒)
	Int64 NextTimeMicroSecond;		// 次タイムアウト時間(マイクロ秒)
	UInt32 StartTimeMilliSecond;	// 開始時間(ミリ秒)
	Boolean SkipFlag;				// スキップフラグ
	UInt32 SkipCounter;				// 連続スキップカウンタ
};

