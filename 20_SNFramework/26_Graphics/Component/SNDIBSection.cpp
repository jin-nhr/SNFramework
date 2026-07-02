#include "SNDIBSection.h"
#include "SNWindow.h"
#include "SNWindowsAPI.h"
#include "SNGDI.h"

// DIBセクションクラス

// コンストラクタ
SNDIBSection::SNDIBSection()
{
	// 変数初期化
	DIBSection = nullptr;
	
	BitmapInfo.StructureSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.Width = 0;
	BitmapInfo.Height = 0;
	BitmapInfo.Planes = 1;
	BitmapInfo.BitCount = 32;
	BitmapInfo.Compression = BI_RGB;
	BitmapInfo.ImageSize = 0;
	BitmapInfo.XPerMeter = 0;
	BitmapInfo.YPerMeter = 0;
	BitmapInfo.ColorUsed = 0;
	BitmapInfo.ColorImportant = 0;

	BitmapInfo.Reserve[0] = 0;
	BitmapInfo.Reserve[1] = 0;
	BitmapInfo.Reserve[2] = 0;
	BitmapInfo.Reserve[3] = 0;

	PixelData = nullptr;

	return;
}

// デストラクタ
SNDIBSection::~SNDIBSection()
{
	// DIBSection有効なら破棄する
	if (DIBSection != nullptr)
	{
		SNGDI gdi;
		gdi.DeleteObject(DIBSection);
		DIBSection = nullptr;
		PixelData = nullptr;

		BitmapInfo.Width = 0;
		BitmapInfo.Height = 0;
		BitmapInfo.ImageSize = 0;
	}

	return;
}

// DIBセクション生成
Void SNDIBSection::CreateDIBSection(Int32 width, Int32 height)
{
	// 破棄しておく
	DeleteDIBSection();

	BitmapInfo.Width = width;
	BitmapInfo.Height = -1 * height;
	BitmapInfo.ImageSize = width * height * 4;
	
	{
		SNGDI gdi;

		// DIBSectionを生成
		DIBSection = gdi.CreateDIBSection(
			(HDC)SNWindow::WindowDC,
			&BitmapInfo,
			(void**)&PixelData);
	}
	return;
}

// DIBセクション破棄
Void SNDIBSection::DeleteDIBSection()
{
	// DIBSection有効なら破棄する
	if (DIBSection != nullptr)
	{
		SNGDI gdi;

		gdi.DeleteObject(DIBSection);
		DIBSection = nullptr;
		PixelData = nullptr;

		BitmapInfo.Width = 0;
		BitmapInfo.Height = 0;
		BitmapInfo.ImageSize = 0;
	}

	return;
}

// ハンドル取得
Handle SNDIBSection::GetDIBSection()
{
	return DIBSection;
}

// ピクセルデータ取得
SNColor* SNDIBSection::GetPixelData()
{
	return PixelData;
}

Int32 SNDIBSection::GetWidth()
{
	return BitmapInfo.Width;
}

Int32 SNDIBSection::GetHeight()
{
	return -1 * BitmapInfo.Height;
}
