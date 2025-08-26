#pragma once
#include "../Include/SNFramework.h"

// FPSタイマクラス

class SNFPSTimer
{
public:
	// コンストラクタ
	SNFPSTimer();

	// デストラクタ
	~SNFPSTimer();

	// FPS設定
	Void SetPFS(UInt32 fps);

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

