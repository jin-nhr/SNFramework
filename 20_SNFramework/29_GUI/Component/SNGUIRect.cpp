#include "SNGUIRect.h"
#include "SNWindowsAPI.h"
#include "SNSystemSurface.h"

// 矩形クラス

// コンストラクタ
SNGUIRect::SNGUIRect()
{
	// 変数初期化
	Surface = &SNSystemSurface::Surface[SNColorBlack];
	Alpha = SNAlphaMax;

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
	Surface = &SNSystemSurface::Surface[SNColorBlack];

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
	if (Alpha == SNAlphaMin)
	{
		// 描画しない
	}

	// アルファ中間値
	else if (Alpha != SNAlphaMax)
	{
		surface->AlphaBlend(
			rect.PointX,
			rect.PointY,
			rect.Width,
			rect.Height,
			Surface->GetDC(),
			0,
			0,
			Surface->GetWidth(),
			Surface->GetHeight(),
			Alpha,
			false);
	}

	// アルファ最大
	else
	{
		surface->StretchBlt(
			rect.PointX,
			rect.PointY,
			rect.Width,
			rect.Height,
			Surface->GetDC(),
			0,
			0,
			Surface->GetWidth(),
			Surface->GetHeight());
	}

	return;
}

// 色設定
Void SNGUIRect::SetColor(SNColorCode color)
{
	Surface = &SNSystemSurface::Surface[color];

	return;
}

// アルファ設定
Void SNGUIRect::SetAlpha(UInt8 alpha)
{
	Alpha = alpha;

	return;
}

// サーフェス設定
Void SNGUIRect::SetSurface(SNSurface* surface)
{
	Surface = surface;

	return;
}
