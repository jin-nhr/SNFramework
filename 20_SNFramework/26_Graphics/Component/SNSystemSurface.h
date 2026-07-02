#pragma once
#include "SNFrameworkInternal.h"
#include "SNSurfaceDDB.h"


// システムサーフェス
class SNSystemSurface
{
public:
	static Void Initialize();
	static Void Terminate();
	static Void CreateSurface();

	// 基本色サーフェス
	static SNSurfaceDDB Surface[SNColorNum];

};

