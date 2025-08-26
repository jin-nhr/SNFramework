#include "SNFPSCounter.h"
#include "../System/SNWindowsAPI.h"
#include "../Library/SNArithmetic.h"
#include "../Configuration/SNConfiguration.h"

// FPSカウンタークラス

// コンストラクタ
SNFPSCounter::SNFPSCounter()
{
	UInt32 measure_time = SNConfiguration::GetInstance()->ConfigurationData.System.FPSMeasureTime;

	// 変数初期化
	NextTimeMilliSecond = 0;
	StartTimeMilliSecond = timeGetTime();
	
	HistoryCount.Allocate(sizeof(UInt32) * measure_time);
	HistoryCount.Clear();

	HistoryIndex = 0;

	TotalCount = 0;
	CurrentCount = 0;

	return;
}

// デストラクタ
SNFPSCounter::~SNFPSCounter()
{
	// メモリ解放
	HistoryCount.Free();

	return;
}

// 測定開始
Void SNFPSCounter::Start()
{
	// 現在時間を取得
	StartTimeMilliSecond = timeGetTime();

	// タイムアウト時間を設定
	NextTimeMilliSecond = SNFPSCounterMilliSecond;

	// カウンタクリア
	CurrentCount = 0;
	TotalCount = 0;
	HistoryCount.Clear();

	return;
}

// カウント
Void SNFPSCounter::Count()
{
	UInt32 measure_time = SNConfiguration::GetInstance()->ConfigurationData.System.FPSMeasureTime;

	// 現在時間取得
	UInt32 now_time = timeGetTime();

	// 経過時間を取得
	UInt32 elapsed_time = (now_time - StartTimeMilliSecond);

	// タイムアウトしたかどうか確認
	if (elapsed_time >= NextTimeMilliSecond)
	{
		// カウンター履歴取得
		UInt32* history_array = (UInt32*)HistoryCount.GetAddress();

		// カウント値の合計を更新する
		// 一番古い値を引いて、現在値を足す
		TotalCount -= history_array[HistoryIndex];
		TotalCount += CurrentCount;

		// 現在のカウント値を履歴に保存する
		history_array[HistoryIndex] = CurrentCount;
		CurrentCount = 0;
		HistoryIndex = (Int32)SNArithmetic::CyclicIncrement(HistoryIndex, 0, measure_time - 1);

		// 次のタイムアウト時間をセットする
		StartTimeMilliSecond = now_time;
		NextTimeMilliSecond = SNFPSCounterMilliSecond - (elapsed_time - SNFPSCounterMilliSecond);
	}

	// カウンタインクリメント
	CurrentCount++;

	return;
}

// FPS取得
UInt32 SNFPSCounter::GetFPS()
{
	UInt32 measure_time = SNConfiguration::GetInstance()->ConfigurationData.System.FPSMeasureTime;

	// 秒間のFPSを計算して返す
	return ((TotalCount + measure_time / 2) / measure_time);
}
