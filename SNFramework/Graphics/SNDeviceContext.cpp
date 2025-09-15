#include "SNDeviceContext.h"
#include "../System/SNSystem.h"
#include "../System/SNWindowsAPI.h"

// デバイスコンテキストクラス

// コンストラクタ
SNDeviceContext::SNDeviceContext()
{
	// 変数初期化
	DeviceContext = nullptr;
	InitialBitmap = nullptr;
	Width = 0;
	Height = 0;

	return;
}

// デストラクタ
SNDeviceContext::~SNDeviceContext()
{
	// デバイスコンテキスト生成済みのときだけ処理
	if (DeviceContext != nullptr)
	{
		// 初期ビットマップ有効なら選択しなおす
		if (InitialBitmap != nullptr)
		{
			// DCに初期ビットマップを選択
			SelectObject((HDC)DeviceContext, (HGDIOBJ)InitialBitmap);
			InitialBitmap = nullptr;
			Width = 1;
			Height = 1;
		}

		// DC削除
		DeleteDC((HDC)DeviceContext);
	}

	return;
}

// DC生成
Void SNDeviceContext::CreateDeviceContext()
{
	// DC削除
	DeleteDeviceContext();

	// ウインドウDC互換のメモリDCを生成
	DeviceContext = CreateCompatibleDC((HDC)SNWindow::WindowDC);
	Width = 1;
	Height = 1;

	// 背景色
	SetBkMode((HDC)DeviceContext, TRANSPARENT);

	// Stretchモード
	SetStretchBltMode((HDC)DeviceContext, SNConfiguration::SystemConfiguration.BltMode);

	// ブラシ座標初期化
	SetBrushOrgEx((HDC)DeviceContext, 0, 0, NULL);

	return;
}

// DC破棄
Void SNDeviceContext::DeleteDeviceContext()
{
	// デバイスコンテキスト生成済みのときだけ処理
	if (DeviceContext != nullptr)
	{
		// ビットマップ選択解除
		ReleaseBitmap();

		// DC削除
		DeleteDC((HDC)DeviceContext);
		DeviceContext = nullptr;
	}

	return;
}

// DC取得
Handle SNDeviceContext::GetDC()
{
	// DCを返す
	return DeviceContext;
}

// ビットマップ選択
Void SNDeviceContext::SetBitmap(Handle bitmap)
{
	BITMAP bmp;

	// ビットマップ選択解除
	ReleaseBitmap();

	// ビットマップを選択
	InitialBitmap = SelectObject((HDC)DeviceContext, (HGDIOBJ)bitmap);

	// ビットマップのサイズ取得
	GetObject((HBITMAP)bitmap, sizeof(BITMAP), &bmp);

	Width = bmp.bmWidth;
	Height = bmp.bmHeight;

	return;
}

// ビットマップ選択解除
Void SNDeviceContext::ReleaseBitmap()
{
	// 初期ビットマップ有効
	if (InitialBitmap != nullptr)
	{
		// DCに初期ビットマップを選択
		SelectObject((HDC)DeviceContext, (HGDIOBJ)InitialBitmap);
		InitialBitmap = nullptr;
		Width = 1;
		Height = 1;
	}

	return;
}

// 矩形転送
Void SNDeviceContext::BitBlt(
	Int32 dst_x, Int32 dst_y,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height)
{
	::BitBlt((HDC)DeviceContext, dst_x, dst_y, src_width, src_height, (HDC)src_dc, src_x, src_y, SRCCOPY);

	return;
}

// 矩形転送(マスクあり)
Void SNDeviceContext::MaskBlt(
	Int32 dst_x, Int32 dst_y,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height,
	Handle mask_dc, Int32 mask_x, Int32 mask_y)
{
	// 抜き色部分を白、描画部分を黒としたマスク画像を対象とする (256色ビットマップなら白：パレット255、黒：パレット0)
	// スプライト画像は背景を黒とする (256色ビットマップならパレット0)
	// AND転送で描画部分を黒で塗りつぶす
	// その後スプライト画像をOR転送する

	::BitBlt((HDC)DeviceContext, dst_x, dst_y, src_width, src_height, (HDC)mask_dc, mask_x, mask_y, SRCAND);
	::BitBlt((HDC)DeviceContext, dst_x, dst_y, src_width, src_height, (HDC)src_dc, src_x, src_y, SRCPAINT);

	return;
}

// 拡大縮小転送
Void SNDeviceContext::StretchBlt(
	Int32 dst_x, Int32 dst_y, Int32 dst_width, Int32 dst_height,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height)
{
	// 拡大縮小転送
	::StretchBlt((HDC)DeviceContext, dst_x, dst_y, dst_width, dst_height, (HDC)src_dc, src_x, src_y, src_width, src_height, SRCCOPY);

	return;
}

// 抜き色転送
Void SNDeviceContext::TransparentBlt(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, SNColor* rgb)
{
	::TransparentBlt((HDC)DeviceContext, x, y, w, h, (HDC)src_dc, src_x, src_y, src_w, src_h, RGB(rgb->Red, rgb->Green, rgb->Blue));
	return;
}

// アルファブレンド
Void SNDeviceContext::AlphaBlend(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, UInt8 alpha, Boolean alpha_ch)
{
	BLENDFUNCTION bf;

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = (alpha_ch ? AC_SRC_ALPHA : 0);

	::AlphaBlend((HDC)DeviceContext, x, y, w, h, (HDC)src_dc, src_x, src_y, src_w, src_h, bf);
	return;
}



// クリア
Void SNDeviceContext::Clear()
{
	// 黒塗りつぶし
	::PatBlt((HDC)DeviceContext, 0, 0, Width, Height, BLACKNESS);

	return;
}

// 白クリア
Void SNDeviceContext::ClearWhite()
{
	// 黒塗りつぶし
	::PatBlt((HDC)DeviceContext, 0, 0, Width, Height, WHITENESS);

	return;
}
