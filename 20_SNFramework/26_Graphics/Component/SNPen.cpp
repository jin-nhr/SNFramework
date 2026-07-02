#include "SNPen.h"
#include "SNWindowsAPI.h"
#include "SNGDI.h"
// ペンクラス

// コンストラクタ
SNPen::SNPen()
{
	Pen = nullptr;

	return;
}

// デストラクタ
SNPen::~SNPen()
{
	if (Pen != nullptr)
	{
		SNGDI gdi;
		gdi.DeleteObject(Pen);
	}

	return;
}

// ペン生成
Void SNPen::CreatePen(SNColor* color)
{
	// 事前に削除しておく
	DeletePen();

	{
		SNGDI gdi;
		Pen = gdi.CreatePen(PS_SOLID, 1, color);
	}
	return;
}

// ペン破棄
Void SNPen::DeletePen()
{
	if (Pen != nullptr)
	{
		SNGDI gdi;
		gdi.DeleteObject(Pen);
		Pen = nullptr;
	}

	return;
}

// ハンドル取得
Handle SNPen::GetHandle()
{
	return Pen;
}

// ライン描画
Void SNPen::DrawLine(SNSurface* surface, SNPoint* point, UInt8 point_num)
{
	SNGDI gdi;

	gdi.DrawLine(surface->GetDC(), point, point_num, Pen);

	return;
}
