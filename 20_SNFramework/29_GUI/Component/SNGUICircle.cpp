#include "SNGUICircle.h"
#include "SNWindowsAPI.h"
#include "SNGraphics.h"
#include "SNSystemBrush.h"
#include "SNSystemPen.h"
#include "SNGDI.h"

// GUI円クラス

// コンストラクタ
SNGUICircle::SNGUICircle()
{
	// 変数初期化
	Color = SNColorBlack;

	return;
}

// デストラクタ
SNGUICircle::~SNGUICircle()
{
	return;
}

// 初期化
Void SNGUICircle::OnInitialize()
{
	Color = SNColorBlack;

	return;
}

// 終了処理
Void SNGUICircle::OnTerminate()
{
	return;
}

// 描画処理
Void SNGUICircle::OnDraw(SNSurface* surface)
{
	SNRect rect = CalcGlobalRect();

	{
		SNGDI gdi;

		gdi.DrawCircle(
			surface->GetDC(),
			rect.PointX,
			rect.PointY,
			rect.Width,
			rect.Height,
			SNSystemBrush::Brush[Color].GetHandle(),
			SNSystemPen::Pen[Color].GetHandle());
	}

	return;
}

// 色設定
Void SNGUICircle::SetColor(SNColorCode color)
{
	Color = color;

	return;
}
