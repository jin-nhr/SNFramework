#include "SNGDI.h"
#include "SNWindowsAPI.h"
#include "SNGraphics.h"
#include "SNSystemConfig.h"

// クリティカルセクション
SNCriticalSection SNGDI::CriticalSectionForGDILock;;

// GDI+
Void* SNGDI::GDIPlusToken = nullptr;

// CLSID
UInt8 SNGDI::EncoderPNG_data[16];
Void* SNGDI::EncoderPNG = nullptr;


// 初期化
Void SNGDI::Initialize()
{
	// GDI+初期化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::Status status = GdiplusStartup((ULONG_PTR*)&GDIPlusToken, &gdiplusStartupInput, nullptr);

	if (GetEncoderClsid((String)L"image/png", (CLSID*)&EncoderPNG_data) == 0)
	{
		// 取得できたらポインタにセット
		EncoderPNG = (Void*)EncoderPNG_data;
	}

	// クリティカルセクション初期化
	CriticalSectionForGDILock.Initialize();

	return;
}

// 終了
Void SNGDI::Terminate()
{
	if (GDIPlusToken != nullptr)
	{
		// GDI+終了
		Gdiplus::GdiplusShutdown((ULONG_PTR)GDIPlusToken);
	}

	return;
}


// エンコーダー取得
Int SNGDI::GetEncoderClsid(String format, Void* clsid)
{
	Int ret = -1;
	UINT num = 0;
	UINT size = 0;
	Gdiplus::ImageCodecInfo* codecinfo;

	// エンコーダー取得
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size != 0)
	{
		codecinfo = (Gdiplus::ImageCodecInfo*)malloc(size);
		if (codecinfo != nullptr)
		{
			GetImageEncoders(num, size, codecinfo);

			for (UINT i = 0; i < num; ++i)
			{
				if (wcscmp((*(codecinfo + i)).MimeType, format) == 0)
				{
					(*(CLSID*)clsid) = (*(codecinfo + i)).Clsid;
					ret = 0;
				}
			}
		}

		free(codecinfo);
	}
	return ret;
}



// GDI
SNGDI::SNGDI() : SNAutoResource(&CriticalSectionForGDILock)
{

	return;
}

// GDI
SNGDI::~SNGDI()
{

	return;
}

// イメージファイルのロード
Handle SNGDI::LoadImageFile(String file_full)
{
	Handle ret = nullptr;
	Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(file_full, FALSE);

	if (bmp != nullptr)
	{
		if (bmp->GetLastStatus() == Gdiplus::Ok)
		{
			HBITMAP hbmp;
			Gdiplus::Color bg(255, 255, 255, 255); // 背景色（透過対策）

			if (bmp->GetHBITMAP(bg, &hbmp) == Gdiplus::Ok)
			{
				ret = (Handle)hbmp;
			}
		}
		delete bmp;
	}

	return ret;
}

// イメージファイルのセーブ
Boolean SNGDI::SaveImageFile(String file_full, Handle image)
{
	Boolean ret = false;
	HBITMAP hbmp = (HBITMAP)image;

	// GDI+ Bitmap に変換
	Gdiplus::Bitmap bmp(hbmp, NULL);

	// PNG エンコーダの CLSID を取得済み？
	if (EncoderPNG != nullptr)
	{
		// 保存
		if (bmp.Save(file_full, (CLSID*)EncoderPNG, NULL) == Gdiplus::Ok)
		{
			ret = true;
		}
	}

	return ret;
}

// DC初期化
Void SNGDI::InitDC(Handle dc)
{
	SetBkMode((HDC)dc, TRANSPARENT);
	SetStretchBltMode((HDC)dc, SNSystemConfig::BltMode);
	SetBrushOrgEx((HDC)dc, 0, 0, NULL);

	return;
}

// メモリDC生成
Handle SNGDI::CreateMemoryDC(Handle base_dc)
{
	return (Handle)CreateCompatibleDC((HDC)base_dc);
}

// DC削除
Void SNGDI::DeleteDC(Handle dc)
{
	::DeleteDC((HDC)dc);
	return;
}

// DDB生成
Handle SNGDI::CreateBitmap(Handle dc, Int32 w, Int32 h)
{
	return ::CreateCompatibleBitmap((HDC)dc, w, h);
}

// DIBSection生成
Handle SNGDI::CreateDIBSection(Handle dc, SNBitmapInfo* bmi, Void** pixel_data)
{
	Handle ret = nullptr;

	// DIBSectionを生成
	ret = (Handle)::CreateDIBSection(
		(HDC)dc,
		(BITMAPINFO*)&bmi,
		DIB_RGB_COLORS,
		(void**)&pixel_data,
		nullptr,
		0);

	return ret;
}

// ビットマップ選択
// もとの選択ビットマップを返す
Handle SNGDI::SetBitmap(Handle dc, Handle bitmap)
{
	return SelectObject((HDC)dc, (HGDIOBJ)bitmap);
}

// ビットマップのサイズ取得
Void SNGDI::GetBitmapSize(Handle bitmap, SNSize* size)
{
	BITMAP bmp;
	GetObject((HBITMAP)bitmap, sizeof(BITMAP), &bmp);

	size->Width = bmp.bmWidth;
	size->Height = bmp.bmHeight;
	
	return;
}

Void SNGDI::PatBlt(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, UInt32 rop)
{
	::PatBlt((HDC)dc, x, y, w, h, rop);

	return;
}


Void SNGDI::BitBlt(
	Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 w, Int32 h, 
	Handle src_dc, Int32 src_x, Int32 src_y, 
	UInt32 rop)
{
	::BitBlt(
		(HDC)dst_dc, dst_x, dst_y, w, h,
		(HDC)src_dc, src_x, src_y,
		rop);

	return;
}

Void SNGDI::StretchBlt(
	Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h, 
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, 
	UInt32 rop)
{
	::StretchBlt(
		(HDC)dst_dc, dst_x, dst_y, dst_w, dst_h,
		(HDC)src_dc, src_x, src_y, src_w, src_h,
		rop);

	return;
}

Void SNGDI::TransparentBlt(
	Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h, 
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h,
	SNColor* rgb)
{
	::TransparentBlt(
		(HDC)dst_dc, dst_x, dst_y, dst_w, dst_h, 
		(HDC)src_dc, src_x, src_y, src_w, src_h, 
		RGB(rgb->Red, rgb->Green, rgb->Blue));
	return;
}

Void SNGDI::AlphaBlend(
	Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h, 
	Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, 
	UInt8 alpha, Boolean alpha_ch)
{
	BLENDFUNCTION bf = { 0 };

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha;
	bf.AlphaFormat = (alpha_ch ? AC_SRC_ALPHA : 0);

	::AlphaBlend((HDC)dst_dc, dst_x, dst_y, dst_w, dst_h, (HDC)src_dc, src_x, src_y, src_w, src_h, bf);

	return;
}



// ブラシ生成
Handle SNGDI::CreateBrush(SNColor* color)
{
	Handle ret;

	ret = (Handle)CreateSolidBrush(RGB(color->Red, color->Green, color->Blue));

	return ret;
}

// ブラシでの塗りつぶし
Void SNGDI::Fill(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, Handle brush)
{
	HGDIOBJ old_brush;

	old_brush = SelectObject((HDC)dc, (HGDIOBJ)brush);

	PatBlt(dc, x, y, w, h, PATCOPY);

	SelectObject((HDC)dc, old_brush);
	
	return;
}

// ペン生成
Handle SNGDI::CreatePen(Int32 style, Int32 width, SNColor* color)
{
	Handle ret = nullptr;

	ret = (Handle)::CreatePen(PS_SOLID, width, RGB(color->Red, color->Green, color->Blue));

	return ret;
}

// ライン描画
Void SNGDI::DrawLine(Handle dc, SNPoint* point, UInt8 point_num, Handle pen)
{
	Int cnt;
	HGDIOBJ old_pen;
	LOGPEN lp;

	if (point_num > 0)
	{
		// ペンの色を取得して始点を描画
		GetObject(pen, sizeof(LOGPEN), &lp);
		

		SetPixel((HDC)dc, point[0].X, point[0].Y, lp.lopnColor);

		// 始点移動
		MoveToEx((HDC)dc, point[0].X, point[0].Y, NULL);

		// ペン選択
		old_pen = SelectObject((HDC)dc, (HGDIOBJ)pen);

		// 始点以降のライン描画
		for (cnt = 1; cnt < point_num; cnt++)
		{
			LineTo((HDC)dc, point[cnt].X, point[cnt].Y);
		}

		// ペンの選択解除
		SelectObject((HDC)dc, old_pen);
	}

	return;
}



// オブジェクト削除
Void SNGDI::DeleteObject(Handle gdi_obj)
{
	::DeleteObject((HGDIOBJ)gdi_obj);

	return;
}


// テキスト描画
Void SNGDI::DrawString(Handle dc, Int32 x, Int32 y, String str, Int32 len, SNColor* color)
{
	::SetTextColor((HDC)dc, RGB(color->Red, color->Green, color->Blue));
	TextOut((HDC)dc, x, y, str, len);

	return;
}

// 円描画
Void SNGDI::DrawCircle(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, Handle brush, Handle pen)
{
	HGDIOBJ old_brush = SelectObject((HDC)dc, (HGDIOBJ)brush);
	HGDIOBJ old_pen = SelectObject((HDC)dc, (HGDIOBJ)pen);

	Ellipse((HDC)dc, x, y, x + w, y + h);

	SelectObject((HDC)dc, old_brush);
	SelectObject((HDC)dc, old_pen);
	return;
}


