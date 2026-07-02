#pragma once
#include "SNFrameworkInternal.h"

// システムカラー

class SNSystemColorTable
{
public:
	static Void Initialize();
	static Void Terminate();

	// システムカラー
	static const SNColor ColorTable[SNColorNum];

};
