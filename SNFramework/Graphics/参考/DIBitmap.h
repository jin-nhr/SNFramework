#pragma once
#include "BitmapInfo.h"

class DIBitmap
{
public:
	DIBitmap();
	virtual ~DIBitmap();

	void Create(int w, int h);
	void Delete();

	Pixel32* GetPixel() { return m_Pixel; }
	void* GetHandle() { return m_hBitmap; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

private:
	void* m_hBitmap;
	void* m_BitmapInfo;
	Pixel32* m_Pixel;
	int m_Width;
	int m_Height;
};
