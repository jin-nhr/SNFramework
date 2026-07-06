#pragma once
#include "SNFrameworkInternal.h"


class SND2D
{
public:
	// ‰Šú‰»
	static Void Initialize();

	// I—¹
	static Void Terminate();

	// •`‰æˆ—
	static Void Draw();


public:
	static Handle D2DFactory;
	static Handle D2DRenderTarget;
	static Handle D2DBrush;
	static Handle DWriteFactory;
	static Handle DWriteTextFormat;
	static Handle WICFactory;



};
