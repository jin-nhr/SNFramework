#include "SNGUIRect.h"
#include "SNWindowsAPI.h"
#include "SNGraphics.h"

// 矩形クラス

// コンストラクタ
SNGUIRect::SNGUIRect()
{
	// 変数初期化
	Color = SNColorBlack;
	Alpha = SNRectAlphaMax;

	return;
}

// デストラクタ
SNGUIRect::~SNGUIRect()
{
	return;
}

// 初期化
Void SNGUIRect::OnInitialize()
{
	Color = SNColorBlack;

	return;
}

// 終了処理
Void SNGUIRect::OnTerminate()
{
	return;
}

// 描画処理
Void SNGUIRect::OnDraw(SNSurface* surface)
{
	SNRect rect = CalcGlobalRect();

	// アルファ0
	if (Alpha == SNRectAlphaMin)
	{
		// 描画しない
	}

	// アルファ中間値
	else if (Alpha != SNRectAlphaMax)
	{
		surface->AlphaBlend(
			rect.PointX,
			rect.PointY,
			rect.Width,
			rect.Height,
			SNGraphics::ColorSurface[Color].GetDC(),
			0,
			0,
			rect.Width,
			rect.Height,
			Alpha,
			false);
	}

	// アルファ最大
	else
	{
		surface->BitBlt(
			rect.PointX,
			rect.PointY,
			SNGraphics::ColorSurface[Color].GetDC(),
			0,
			0,
			rect.Width,
			rect.Height);
	}

	return;
}

// 色設定
Void SNGUIRect::SetColor(SNColorCode color)
{
	Color = color;

	return;
}

// アルファ設定
Void SNGUIRect::SetAlpha(UInt8 alpha)
{
	Alpha = alpha;

	return;
}
