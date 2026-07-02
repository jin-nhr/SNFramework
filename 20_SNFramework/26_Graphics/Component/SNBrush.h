#pragma once
#include "SNFrameworkInternal.h"
#include "SNSurface.h"

// ブラシクラス

class SNBrush
{
public:
	// コンストラクタ
	SNBrush();

	// デストラクタ
	~SNBrush();

	// ブラシ生成
	Void CreateBrush(SNColor* color);

	// ブラシ破棄
	Void DeleteBrush();

	// ハンドル取得
	Handle GetHandle();

	// 塗りつぶし
	Void Fill(SNSurface* surface, Int32 x, Int32 y, Int32 w, Int32 h);

private:
	Handle Brush;	// ブラシハンドル
};
