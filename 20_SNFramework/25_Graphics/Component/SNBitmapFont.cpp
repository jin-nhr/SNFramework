#include "SNBitmapFont.h"
#include "SNGraphicsDevice.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"
#include "SNConfig.h"
#include "SNGraphics.h"
#include "SNGraphicsResManager.h"

// コード定義
#include "SNBMCharTable.h"

// ビットマップフォントクラス


SNColorMatrix SNBitmapFont::ColorMatrix;
SNStore SNBitmapFont::WorkStore;


// 初期化処理
Void SNBitmapFont::Initialize()
{
	SNGraphicsResManager::AccessGet(SNGraphicsResSystemFont);

	return;
}

// 拡張機能初期化
Void SNBitmapFont::InitializeExtraFunc()
{
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;

	SNGraphicsResManager::AccessGet(SNGraphicsResExFont01);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont02);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont03);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont04);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont05);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont06);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont07);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont08);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont09);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont10);
	SNGraphicsResManager::AccessGet(SNGraphicsResExFont11);

	// カラーマトリクス準備
	grc->CreateColorMatrix(&ColorMatrix);

	// ストア生成
	WorkStore.CreateResourceFunc = CreateWorkSurface;
	WorkStore.DeleteResourceFunc = DeleteWorkSurface;
	WorkStore.CreateStore(SNSystemConfig::BMFontWorkNum);

	return;
}

// 終了処理
Void SNBitmapFont::Terminate()
{
	ColorMatrix.DeleteColorMatrix();
	WorkStore.DeleteStore();

	SNGraphicsResManager::AccessRelease(SNGraphicsResSystemFont);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont01);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont02);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont03);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont04);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont05);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont06);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont07);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont08);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont09);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont10);
	SNGraphicsResManager::AccessRelease(SNGraphicsResExFont11);

	return;
}

// 文字列描画
Void SNBitmapFont::DrawSystemText(SNGraphicsContext* dst_dc, Int32 x, Int32 y, BMString str, UInt32 len)
{
	UInt32 cnt;
	SNPoint pt;
	SNRect dst_rect;
	SNRect src_rect;
	UInt8 page;
	SNBitmap* font_surface[] =
	{
		SNGraphicsResManager::GetResource(SNGraphicsResSystemFont),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont01),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont02),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont03),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont04),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont05),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont06),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont07),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont08),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont09),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont10),
		SNGraphicsResManager::GetResource(SNGraphicsResExFont11),
	};

	dst_rect.PointX = x;
	dst_rect.PointY = y;
	dst_rect.Width = BMCharWidth;
	dst_rect.Height = BMCharHeight;
	src_rect.Width = BMCharWidth;
	src_rect.Height = BMCharHeight;

	for (cnt = 0; cnt < len; cnt++)
	{
		// フォント座標取得
		pt = GetBMCharPoint(str[cnt]);

		src_rect.PointX = pt.X;
		src_rect.PointY = pt.Y;

		page = GetBMCharPage(str[cnt]);

		// 描画
		dst_dc->DrawImage(&dst_rect, font_surface[page], &src_rect, SNAlphaMax);

		// 1文字分描画位置をずらす
		dst_rect.PointX += BMCharWidth;
	}

	return;
}

// ストアからワーク取得
SNListContainer* SNBitmapFont::GetWorkSurface()
{
	return WorkStore.GetResource();
}

// ワーク解放
Void SNBitmapFont::ReleaseWorkSurface(SNListContainer* work)
{
	WorkStore.ReleaseResource(work);

	return;
}

// テキストのプレ描画
Void SNBitmapFont::PreDrawText(SNListContainer* work, BMString str, UInt32 len)
{
	SNBitmap* bmp = (SNBitmap*)work->UserData;
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNColor color = { 0, 0, 0, 0 };

	if (bmp != nullptr)
	{
		// ワークに対してテキスト描画
		grc->Begin(bmp);

		grc->Clear(&color);
		DrawSystemText(grc, 0, 0, str, len);

		grc->End();
	}

	return;
}

// 拡張テキスト描画
Void SNBitmapFont::DrawExtraText(SNGraphicsContext* dst_dc, SNListContainer* work, Int32 x, Int32 y, SNColor* color, UInt32 len)
{
	SNPoint pnt =
	{
		x,
		y
	};

	SNRect rect =
	{
		0,
		0,
		(Int32)(BMCharWidth * len),
		(Int32)BMCharHeight
	};

	ColorMatrix.ClearMatrix();
	ColorMatrix.Multiply(color);
	ColorMatrix.SetMatrix();
	ColorMatrix.SetSource((SNBitmap*)work->UserData);

	dst_dc->ColorMatrixEffect(&pnt, &rect, &ColorMatrix);

	return;
}

// String→BMString
Void SNBitmapFont::StringToBMString(String str, Int32 len, BMString bmstr, Int32 bmlen)
{
	Int32 cnt;
	Int32 cnv_len = (Int32)SNMath::SelectMin(len, bmlen);
	
	// 変換文字数分、Char→BMChar変換
	for (cnt = 0; cnt < cnv_len; cnt++)
	{
		bmstr[cnt] = SNCharToBMChar(str[cnt]);
	}

	return;
}

// BMChar→Char変換
Char SNBitmapFont::BMCharToChar(BMChar bmch)
{
	UInt8 page = GetBMCharPage(bmch);
	UInt8 code = GetBMCharCode(bmch);
	Char ret_chr = L'　';
	
	if (page < BMPageNum)
	{
		ret_chr = SNBMCharTable[page][code];
	}
	
	
	return ret_chr;
}

// BMString→String変換
Void SNBitmapFont::BMStringToString(BMString bmstr, Int32 bmlen, String str, Int32 len)
{
	Int32 cnt;
	Int32 cnv_len = (Int32)SNMath::SelectMin(len, bmlen);

	// 変換文字数分、BMChar→Char変換
	for (cnt = 0; cnt < cnv_len; cnt++)
	{
		str[cnt] = BMCharToChar(bmstr[cnt]);
	}

	return;
}


// ビットマップフォント座標取得
SNPoint SNBitmapFont::GetBMCharPoint(BMChar bmch)
{
	SNPoint pnt;
	UInt32 code = GetBMCharCode(bmch);

	pnt.X = (code % BMCharBlockNumX) * BMCharWidth;
	pnt.Y = (code / BMCharBlockNumX) * BMCharHeight;

	return pnt;
}

// ビットマップフォントページ取得
UInt8 SNBitmapFont::GetBMCharPage(BMChar bmch)
{
	UInt8 page = (((bmch & 0xFF00) >> 8) & 0x00FF);

	return page;
}

// ビットマップフォントコード取得
UInt8 SNBitmapFont::GetBMCharCode(BMChar bmch)
{
	UInt8 code = (bmch & 0x00FF);

	return code;
}

Void* SNBitmapFont::CreateWorkSurface()
{
	SNBitmap* bmp = new SNBitmap;
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNSize size;

	// 画面の横幅 x フォントの高さのサーフェスにする
	size.Width = SNSystemConfig::ScreenWidth;
	size.Height = BMCharHeight;

	grc->CreateBitmap(bmp, &size);

	return bmp;
}

Void SNBitmapFont::DeleteWorkSurface(Void* res)
{
	SNBitmap* bmp = (SNBitmap*)res;

	bmp->DeleteBitmap();

	delete bmp;

	return;
}
