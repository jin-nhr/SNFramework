#pragma once
#include "SNFrameworkInternal.h"
#include "SNPen.h"

//システムペン

class SNSystemPen
{
public:
	static Void Initialize();
	static Void Terminate();

	// ペン
	static SNPen Pen[SNColorNum];

};
