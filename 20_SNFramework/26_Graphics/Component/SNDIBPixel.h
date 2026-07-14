#pragma once
#include "SNFrameworkInternal.h"

// DIBPixelクラス

class SNDIBPixel
{
public:
	// コンストラクタ
	SNDIBPixel(Handle lock);

	// デストラクタ
	virtual ~SNDIBPixel();

	virtual SNColor* Ref(Int32 x, Int32 y);

	virtual Int32 GetStride();

	virtual Int32 GetDataSize();

private:
	Handle BitmapLock;
	SNColor* PixelPtr;
	Int32 DataSize;
	Int32 Stride;
};
