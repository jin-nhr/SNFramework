#pragma once
#include "BitmapInfo.h"


class BitmapInfo8
{
public:
	const static int ColorBits = 8;
	const static int ColorPaletteNum = 256;

};


// PIXEL 8bit データ
typedef unsigned char		Pixel8;


// 8bitカラー用ColorPalette
struct ColorPalette
{
	Pixel32	Color[BitmapInfo8::ColorPaletteNum];
};

