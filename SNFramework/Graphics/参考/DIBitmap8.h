#pragma once
#include "BitmapInfo8.h"

class DIBitmap8
{
public:
	DIBitmap8();
	virtual ~DIBitmap8();

	void Create(int w, int h);
	void Delete();

	Pixel8* GetPixel() { return m_Pixel; }
	void* GetHandle() { return m_hBitmap; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

private:
	unsigned char* m_BitmapInfoAddress;
	void* m_hBitmap;
	void* m_BitmapInfo;
	ColorPalette* m_ColorPalette;
	Pixel8* m_Pixel;
	int m_Width;
	int m_Height;
};
