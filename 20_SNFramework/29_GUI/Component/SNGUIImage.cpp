#include "SNGUIImage.h"
#include "SNWindowsAPI.h"
#include "SNGraphics.h"

// 矩形クラス

// コンストラクタ
SNGUIImage::SNGUIImage()
{
	// 変数初期化
	Alpha = SNAlphaMax;
	Surface = nullptr;

	return;
}

// デストラクタ
SNGUIImage::~SNGUIImage()
{
	return;
}

// 初期化
Void SNGUIImage::OnInitialize()
{


	return;
}

// 終了処理
Void SNGUIImage::OnTerminate()
{
	return;
}

// サーフェス設定
Void SNGUIImage::SetSurface(SNSurface* surface)
{
	Surface = surface;

	return;
}

// アルファ設定
Void SNGUIImage::SetAlpha(UInt8 alpha)
{
	Alpha = alpha;

	return;
}

// 描画範囲調整
Void SNGUIImage::AdjustDrawRect(SNRect* dst, SNRect* src)
{
	Int64 new_width;
	Int64 new_heigth;
	Int64 offset_x;
	Int64 offset_y;
	Float64 src_aspect;
	Float64 dst_aspect;

	// src のアスペクト比を計算
	src_aspect = (Float64)(src->Width) / (Float64)(src->Height);
	dst_aspect = (Float64)(dst->Width) / (Float64)(dst->Height);

	// dst のサイズを調整
	if (dst_aspect > src_aspect)
	{
		// dst が横長すぎる → 横幅を縮める
		new_width = (Int64)(dst->Height * src_aspect);
		offset_x = (dst->Width - new_width) / 2;
		dst->PointX += (Int32)offset_x;
		dst->Width = (Int32)new_width;
	}
	else
	{
		// dst が縦長すぎる → 高さを縮める
		new_heigth = (Int64)(dst->Width / src_aspect);
		offset_y = (dst->Height - new_heigth) / 2;
		dst->PointY += (Int32)offset_y;
		dst->Height = (Int32)new_heigth;
	}

	return;
}

// 描画処理
Void SNGUIImage::OnDraw(SNSurface* surface)
{
	SNRect rect = CalcGlobalRect();
	SNRect src_rect;

	// サーフェス設定あり
	if (Surface != nullptr)
	{
		src_rect.PointX = 0;
		src_rect.PointY = 0;
		src_rect.Width = Surface->GetWidth();
		src_rect.Height = Surface->GetHeight();

		// 描画範囲調整
		AdjustDrawRect(&rect, &src_rect);

		// アルファ0
		if (Alpha == SNAlphaMin)
		{
			// 描画しない
		}

		// Alpha最大
		else if (Alpha == SNAlphaMax)
		{
			surface->StretchBlt(rect.PointX, rect.PointY, rect.Width, rect.Height,
				Surface->GetDC(), src_rect.PointX, src_rect.PointY, src_rect.Width, src_rect.Height);
		}

		// Alpha中間値
		else
		{
			surface->AlphaBlend(rect.PointX, rect.PointY, rect.Width, rect.Height,
				Surface->GetDC(), src_rect.PointX, src_rect.PointY, src_rect.Width, src_rect.Height, Alpha, false);
		}
	}

	return;
}
