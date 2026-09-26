#include "SNBitmapFont.h"
#include "SNGraphicsDevice.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"
#include "SNConfig.h"
#include "SNGraphics.h"
#include "SNGraphicsResManager.h"
#include "SNGUIDef.h"

// コード定義
#include "SNBMCharTable.h"

// ビットマップフォントクラス


// 初期化処理
Void SNBitmapFont::Initialize()
{
	SNGraphicsResManager::AccessGet(SNGraphicsResGUI);

	return;
}

// 拡張機能初期化
Void SNBitmapFont::InitializeExtraFunc()
{
	return;
}

// 終了処理
Void SNBitmapFont::Terminate()
{
	SNGraphicsResManager::AccessRelease(SNGraphicsResGUI);
	return;
}

// 文字列描画
Void SNBitmapFont::DrawSystemText(Int32 x, Int32 y, BMString str, UInt32 len, SNColor* color)
{
	UInt32 cnt;
	SNPoint pt;
	SNRect dst_rect;
	SNRect src_rect;
	SNBitmap* font_surface = SNGraphicsResManager::GetResource(SNGraphicsResGUI);

	dst_rect.PointX = x;
	dst_rect.PointY = y;
	dst_rect.Width = SNGUIDef::BMCharWidth;
	dst_rect.Height = SNGUIDef::BMCharHeight;
	src_rect.Width = SNGUIDef::BMCharWidth;
	src_rect.Height = SNGUIDef::BMCharHeight;

	for (cnt = 0; cnt < len; cnt++)
	{
		// フォント座標取得
		pt = GetBMCharPoint(str[cnt]);

		src_rect.PointX = pt.X;
		src_rect.PointY = pt.Y;

		// 描画
		SNGraphicsDevice::DrawImage(&dst_rect, font_surface, &src_rect, SNAlphaMax, color);

		// 1文字分描画位置をずらす
		dst_rect.PointX += SNGUIDef::BMCharWidth;
	}

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
	UInt32 page = GetBMCharPage(bmch);

	pnt.X = SNGUIDef::BMCharBaseX + (code % SNGUIDef::BMCharBlockNumX) * SNGUIDef::BMCharWidth + SNGUIDef::BMCharPageOffsetX * page;
	pnt.Y = SNGUIDef::BMCharBaseY + (code / SNGUIDef::BMCharBlockNumX) * SNGUIDef::BMCharHeight + SNGUIDef::BMCharPageOffsetY * page;

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
