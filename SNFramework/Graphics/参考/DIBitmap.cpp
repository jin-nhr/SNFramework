#include "DIBitmap.h"
#include "Graphics.h"

#include "framework.h"


DIBitmap::DIBitmap()
{
	m_hBitmap = NULL;
	m_BitmapInfo = NULL;
	m_Pixel = NULL;
	m_Width = 0;
	m_Height = 0;

	return;
}

DIBitmap::~DIBitmap()
{
	Delete();

	return;
}

void DIBitmap::Create(int w, int h)
{
	BITMAPINFOHEADER* bmph = new BITMAPINFOHEADER;

	Delete();

	m_Width = w;
	m_Height = h;

	bmph->biSize = sizeof(BITMAPINFOHEADER);
	bmph->biWidth = w;
	bmph->biHeight = h;
	bmph->biCompression = BI_RGB;
	bmph->biPlanes = 1;
	bmph->biBitCount = BitmapInfo::ColorBits;
	bmph->biClrImportant = 0;
	bmph->biSizeImage = w * h * (BitmapInfo::ColorBits  / 8);
	bmph->biClrUsed = 0;
	bmph->biXPelsPerMeter = 0;
	bmph->biYPelsPerMeter = 0;

	m_BitmapInfo = (void*)bmph;

	m_hBitmap = (void*)CreateDIBSection(
		(HDC)Graphics::GetScreenDC(),
		(BITMAPINFO*)m_BitmapInfo,
		DIB_RGB_COLORS,
		(void**)&m_Pixel,
		0,
		0);

	return;
}

void DIBitmap::Delete()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject((HGDIOBJ)m_hBitmap);
		delete m_BitmapInfo;
		m_BitmapInfo = NULL;
		m_hBitmap = NULL;
	}

	return;
}



