#include "SNGUIPicture.h"



// コンストラクタ
SNGUIPicture::SNGUIPicture()
{
	Bitmap = nullptr;
	return;
}

// デストラクタ
SNGUIPicture::~SNGUIPicture()
{
	return;
}

// ビットマップ設定
Void SNGUIPicture::SetBitmap(SNBitmap* bmp)
{
	Bitmap = bmp;
	return;
}

// スケーリング設定
Void SNGUIPicture::SetScaling(SNScalingType scale)
{
	Scaling = scale;
	return;
}

// 初期化
Void SNGUIPicture::OnInitialize()
{
	return;
}

// 終了処理
Void SNGUIPicture::OnTerminate()
{
	return;
}

// 描画前処理
Void SNGUIPicture::OnPreDraw()
{
	return;
}

// 描画処理
Void SNGUIPicture::OnDraw(SNGraphicsContext* grc)
{
	SNSize bmp_size;
	SNRect dst_rect;
	SNRect src_rect;

	if (Bitmap != nullptr)
	{
		Bitmap->GetSize(&bmp_size);

		src_rect.PointX = 0;
		src_rect.PointY = 0;
		src_rect.Width = bmp_size.Width;
		src_rect.Height = bmp_size.Height;

		dst_rect = GetRect();

		grc->DrawImage(&dst_rect, Bitmap, &src_rect, SNAlphaMax);
	}
	return;
}

// 描画後処理
Void SNGUIPicture::OnPostDraw()
{
	return;
}
