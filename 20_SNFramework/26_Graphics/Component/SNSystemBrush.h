#pragma once
#include "SNFrameworkInternal.h"
#include "SNBrush.h"


//システムブラシ

class SNSystemBrush
{
public:
	static Void Initialize();
	static Void Terminate();
	static Void CreateBrush();

	// ブラシ
	static SNBrush Brush[SNColorNum];

};

