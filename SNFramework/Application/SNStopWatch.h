#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNMemory.h"

// ストップウォッチクラス

class SNStopWatch
{
public:
	// コンストラクタ
	SNStopWatch();

	// デストラクタ
	~SNStopWatch();

	// 測定開始
	// パラメータ true:過去データクリア、false:過去データを残す
	Void Start(Boolean clear);

	// 停止
	Void Stop();

	// 平均値取得(ミリ秒単位)
	UInt32 GetAverage();

	// 最大値取得(ミリ秒単位)
	UInt32 GetMax();

	// 最小値取得(ミリ秒単位)
	UInt32 GetMin();

private:
	UInt32 StartCount;		// 開始時間
	UInt32 MaxTime;			// 最大時間
	UInt32 MinTime;			// 最小時間
	SNMemory TimeHistory;	// 時間履歴
	UInt64 TotalTime;		// 合計時間
	UInt32 HistoryIndex;	// 履歴インデックス
	Boolean RunState;		// 動作状態
};
