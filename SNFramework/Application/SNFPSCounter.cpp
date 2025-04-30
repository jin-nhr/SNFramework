#include "SNFPSCounter.h"
#include "../System/SNWindowsAPI.h"
#include "../Library/SNArithmetic.h"

// FPSカウンタークラス

#define SNFPSCOUNTER_MILLISECOND	1000	// 1秒→マイクロ秒変換値
#define SNFPSCOUNTER_MEASURE_TIME	10		// 測定時間(秒)

// コンストラクタ
SNFPSCounter::SNFPSCounter()
{
	// 変数初期化
	NextTimeMilliSecond = 0;
	StartTimeMilliSecond = timeGetTime();
	
	HistoryCount.Allocate(sizeof(UInt32) * SNFPSCOUNTER_MEASURE_TIME);
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
	NextTimeMilliSecond = SNFPSCOUNTER_MILLISECOND;

	// カウンタクリア
	CurrentCount = 0;
	TotalCount = 0;
	HistoryCount.Clear();

	return;
}

// カウント
Void SNFPSCounter::Count()
{
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
		HistoryIndex = SNArithmetic::CyclicIncrement(HistoryIndex, 0, SNFPSCOUNTER_MEASURE_TIME - 1);

		// 次のタイムアウト時間をセットする
		StartTimeMilliSecond = now_time;
		NextTimeMilliSecond = SNFPSCOUNTER_MILLISECOND - (elapsed_time % SNFPSCOUNTER_MILLISECOND);
	}

	// カウンタインクリメント
	CurrentCount++;

	return;
}

// FPS取得
UInt32 SNFPSCounter::GetFPS()
{
	// 秒間のFPSを計算して返す
	return ((TotalCount + SNFPSCOUNTER_MEASURE_TIME / 2) / SNFPSCOUNTER_MEASURE_TIME);
}

