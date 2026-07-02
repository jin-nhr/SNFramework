#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"

// FPSカウンタークラス
// 秒間のフレーム実行数をカウントし
// 指定した測定時間内の平均値を算出する

class SNFPSCounter
{
public:
	static constexpr Int32 SNFPSCounterMilliSecond = 1000;	// 1秒→マイクロ秒変換値
	static constexpr UInt32 FPSMeasureTime = 10;			// FPS測定時間(秒)

public:
	// コンストラクタ
	SNFPSCounter();

	// デストラクタ
	~SNFPSCounter();

	// 測定開始
	Void Start();

	// カウント
	Void Count();

	// FPS取得
	UInt32 GetFPS();

private:
	UInt32 NextTimeMilliSecond;		// 次タイムアウト時間(ミリ秒)
	UInt32 StartTimeMilliSecond;	// 開始時間(ミリ秒)
	SNMemory HistoryCount;			// カウント値履歴
	Int32 HistoryIndex;				// カウント値履歴インデックス
	UInt32 TotalCount;				// カウント値合計
	UInt32 CurrentCount;			// 現在カウント値
};

