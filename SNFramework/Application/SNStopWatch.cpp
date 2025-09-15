#include "SNStopWatch.h"
#include "../System/SNWindowsAPI.h"
#include "../Library/SNMath.h"
#include "../Configuration/SNConfiguration.h"

// コンストラクタ
SNStopWatch::SNStopWatch()
{
	UInt32 history = SNConfiguration::SystemConfiguration.StopWatchHistory;

	// 変数初期化
	StartCount = 0;
	MaxTime = 0;
	MinTime = (UInt32)(-1);
	TimeHistory.Allocate(sizeof(UInt32) * history);
	TimeHistory.Clear();
	TotalTime = 0;
	HistoryIndex = 0;
	RunState = false;

	return;
}

// デストラクタ
SNStopWatch::~SNStopWatch()
{
	// 変数初期化
	TimeHistory.Free();
	return;
}

// 測定開始
// パラメータ true:過去データクリア、false:過去データを残す
Void SNStopWatch::Start(Boolean clear)
{
	// クリアフラグ指定時
	if (clear)
	{
		MaxTime = 0;
		MinTime = (UInt32)(-1);
		TotalTime = 0;
		TimeHistory.Clear();
		HistoryIndex = 0;
	}

	// 開始時間を記録
	StartCount = timeGetTime();

	// 動作中
	RunState = true;

	return;
}

// 停止
Void SNStopWatch::Stop()
{
	UInt32 history = SNConfiguration::SystemConfiguration.StopWatchHistory;

	// 動作中のみ処理を実行
	if (RunState)
	{
		UInt32 now_time = timeGetTime();	// 現在時間
		UInt32 time = (UInt32)(now_time - StartCount);	// 経過時間計算
		UInt32* history_array = (UInt32*)TimeHistory.GetAddress();	// 履歴取得

		// 合計時間の計算
		TotalTime -= history_array[HistoryIndex];
		TotalTime += time;

		// 現在のカウント値を履歴に保存する
		history_array[HistoryIndex] = time;
		HistoryIndex = (Int32)SNMath::Increment(HistoryIndex, 0, history - 1);

		// 最大、最小を更新
		MaxTime = (UInt32)SNMath::SelectMax(MaxTime, time);
		MinTime = (UInt32)SNMath::SelectMin(MinTime, time);

		// 停止状態
		RunState = false;
	}

	return;
}

// 平均値取得(ミリ秒単位)
UInt32 SNStopWatch::GetAverage()
{
	UInt32 history = SNConfiguration::SystemConfiguration.StopWatchHistory;

	// 合計時間を履歴数でわる
	return (UInt32)(TotalTime / history);
}

// 最大値取得(ミリ秒単位)
UInt32 SNStopWatch::GetMax()
{
	return MaxTime;
}

// 最小値取得(ミリ秒単位)
UInt32 SNStopWatch::GetMin()
{
	return MinTime;
}

