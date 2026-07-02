#include "SNDeviceContext.h"
#include "SNSystemConfig.h"
#include "SNWindow.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNGDI.h"

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
		SNGDI gdi;

		// 初期ビットマップ有効なら選択しなおす
		if (InitialBitmap != nullptr)
		{
			// DCに初期ビットマップを選択
			gdi.SetBitmap(DeviceContext, InitialBitmap);
			InitialBitmap = nullptr;
			Width = 1;
			Height = 1;
		}

		// DC削除
		gdi.DeleteDC(DeviceContext);
	}

	return;
}

// DC生成
Void SNDeviceContext::CreateDeviceContext()
{
	// DC削除
	DeleteDeviceContext();

	// DC生成と初期設定
	{
		SNGDI gdi;

		// ウインドウDC互換のメモリDCを生成
		DeviceContext = gdi.CreateMemoryDC(SNWindow::WindowDC);

		gdi.InitDC(DeviceContext);
	}

	Width = 1;
	Height = 1;

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

		{
			SNGDI gdi;

			// DC削除
			gdi.DeleteDC(DeviceContext);
		}
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
	SNSize size;

	// ビットマップ選択解除
	ReleaseBitmap();

	// ビットマップ選択とサイズ取得
	{
		SNGDI gdi;

		// ビットマップを選択
		InitialBitmap = gdi.SetBitmap(DeviceContext, bitmap);

		// ビットマップのサイズ取得
		gdi.GetBitmapSize(bitmap, &size);
	}

	Width = size.Width;
	Height = size.Height;

	return;
}

// ビットマップ選択解除
Void SNDeviceContext::ReleaseBitmap()
{
	// 初期ビットマップ有効
	if (InitialBitmap != nullptr)
	{
		{
			SNGDI gdi;
			// DCに初期ビットマップを選択
			gdi.SetBitmap(DeviceContext, InitialBitmap);
		}
		
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
	SNGDI gdi;
	
	gdi.BitBlt(DeviceContext, dst_x, dst_y, src_width, src_height, src_dc, src_x, src_y, SRCCOPY);

	return;
}

// AND転送
Void SNDeviceContext::AndBlt(
	Int32 dst_x, Int32 dst_y,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height)
{
	SNGDI gdi;
	
	gdi.BitBlt(DeviceContext, dst_x, dst_y, src_width, src_height, src_dc, src_x, src_y, SRCAND);

	return;
}

// 反転転送
Void SNDeviceContext::InvertBlt(
	Int32 dst_x, Int32 dst_y,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height)
{
	SNGDI gdi;

	gdi.BitBlt(DeviceContext, dst_x, dst_y, src_width, src_height, src_dc, src_x, src_y, NOTSRCCOPY);

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

	SNGDI gdi;

	gdi.BitBlt(DeviceContext, dst_x, dst_y, src_width, src_height, mask_dc, mask_x, mask_y, SRCAND);
	gdi.BitBlt(DeviceContext, dst_x, dst_y, src_width, src_height, src_dc, src_x, src_y, SRCPAINT);

	return;
}

// 拡大縮小転送
Void SNDeviceContext::StretchBlt(
	Int32 dst_x, Int32 dst_y, Int32 dst_width, Int32 dst_height,
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height)
{
	SNGDI gdi;

	// 拡大縮小転送
	gdi.StretchBlt(DeviceContext, dst_x, dst_y, dst_width, dst_height, src_dc, src_x, src_y, src_width, src_height, SRCCOPY);

	return;
}

// 抜き色転送
Void SNDeviceContext::TransparentBlt(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, SNColor* rgb)
{
	SNGDI gdi;

	gdi.TransparentBlt(DeviceContext, x, y, w, h, src_dc, src_x, src_y, src_w, src_h, rgb);
	return;
}

// アルファブレンド
Void SNDeviceContext::AlphaBlend(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, UInt8 alpha, Boolean alpha_ch)
{
	SNGDI gdi;

	gdi.AlphaBlend(DeviceContext, x, y, w, h, src_dc, src_x, src_y, src_w, src_h, alpha, alpha_ch);

	return;
}


// クリア
Void SNDeviceContext::Clear()
{
	SNGDI gdi;
	
	// 黒塗りつぶし
	gdi.PatBlt(DeviceContext, 0, 0, Width, Height, BLACKNESS);

	return;
}

// 白クリア
Void SNDeviceContext::ClearWhite()
{
	SNGDI gdi;

	// 黒塗りつぶし
	gdi.PatBlt(DeviceContext, 0, 0, Width, Height, WHITENESS);

	return;
}
