#include "SNBrush.h"
#include "SNGDI.h"

// コンストラクタ
SNBrush::SNBrush()
{
	Brush = nullptr;

	return;
}

// デストラクタ
SNBrush::~SNBrush()
{
	if (Brush != nullptr)
	{
		SNGDI gdi;
		gdi.DeleteObject(Brush);
	}

	return;
}

// ブラシ生成
Void SNBrush::CreateBrush(SNColor* color)
{
	// 事前に削除
	DeleteBrush();

	{
		SNGDI gdi;
		// ブラシ生成
		Brush = gdi.CreateBrush(color);
	}
	return;
}

// ブラシ破棄
Void SNBrush::DeleteBrush()
{
	if (Brush != nullptr)
	{
		SNGDI gdi;
		gdi.DeleteObject(Brush);
		Brush = nullptr;
	}

	return;
}

// ハンドル取得
Handle SNBrush::GetHandle()
{
	return Brush;
}

// 塗りつぶし
Void SNBrush::Fill(SNSurface* surface, Int32 x, Int32 y, Int32 w, Int32 h)
{
	SNGDI gdi;

	gdi.Fill(surface->GetDC(),x, y, w, h ,Brush);

	return;
}
