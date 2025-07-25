#include "DIBitmap8.h"
#include "Graphics.h"

#include "framework.h"


DIBitmap8::DIBitmap8()
{
	m_BitmapInfoAddress = NULL;
	m_hBitmap = NULL;
	m_BitmapInfo = NULL;
	m_ColorPalette = NULL;
	m_Pixel = NULL;
	m_Width = 0;
	m_Height = 0;

	return;
}

DIBitmap8::~DIBitmap8()
{
	Delete();

	return;
}

void DIBitmap8::Create(int w, int h)
{
	BITMAPINFO* bmpinf =
		(BITMAPINFO*)(new unsigned char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * BitmapInfo8::ColorPaletteNum]);

	BITMAPINFOHEADER* bmph = &bmpinf->bmiHeader;
	RGBQUAD* palette = bmpinf->bmiColors;

	Delete();

	m_Width = w;
	m_Height = h;

	bmph->biSize = sizeof(BITMAPINFOHEADER);
	bmph->biWidth = w;
	bmph->biHeight = h;
	bmph->biCompression = BI_RGB;
	bmph->biPlanes = 1;
	bmph->biBitCount = BitmapInfo8::ColorBits;
	bmph->biClrImportant = 0;
	bmph->biSizeImage = w * h * (BitmapInfo8::ColorBits / 8);
	bmph->biClrUsed = 0;
	bmph->biXPelsPerMeter = 0;
	bmph->biYPelsPerMeter = 0;

	ZeroMemory((void*)palette, sizeof(RGBQUAD) * BitmapInfo8::ColorPaletteNum);


	m_BitmapInfoAddress = (unsigned char*)bmpinf;
	m_BitmapInfo = (void*)bmph;
	m_ColorPalette = (ColorPalette*)palette;


	m_hBitmap = (void*)CreateDIBSection(
		(HDC)Graphics::GetScreenDC(),
		(BITMAPINFO*)m_BitmapInfo,
		DIB_RGB_COLORS,
		(void**)&m_Pixel,
		0,
		0);

	return;
}

void DIBitmap8::Delete()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject((HGDIOBJ)m_hBitmap);
		delete[] m_BitmapInfoAddress;
		m_BitmapInfo = NULL;
		m_hBitmap = NULL;
		m_ColorPalette = NULL;
		m_BitmapInfoAddress = NULL;
	}

	return;
}
