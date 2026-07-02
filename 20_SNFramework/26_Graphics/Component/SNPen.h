#pragma once
#include "SNFrameworkInternal.h"
#include "SNSurface.h"

// ペンクラス

class SNPen
{
public:
	// コンストラクタ
	SNPen();

	// デストラクタ
	~SNPen();

	// ペン生成
	Void CreatePen(SNColor* color);

	// ペン破棄
	Void DeletePen();

	// ハンドル取得
	Handle GetHandle();

	// ライン描画
	Void DrawLine(SNSurface* surface, SNPoint* point, UInt8 point_num);

private:
	Handle Pen;	// ペンハンドル

};
