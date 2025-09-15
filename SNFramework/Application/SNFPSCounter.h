#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNMemory.h"

// FPSカウンタークラス

class SNFPSCounter
{
public:
	static const Int32 SNFPSCounterMilliSecond;

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

