#pragma once
#include "SNFrameworkInternal.h"
#include "SNDeviceContext.h"

// サーフェスクラス
class SNSurface : public SNDeviceContext
{
public:
	// コンストラクタ
	SNSurface();

	// デストラクタ
	virtual ~SNSurface();

	// サーフェス生成
	virtual Void CreateSurface(Int32 width, Int32 height) = 0;

	// サーフェス破棄
	virtual Void DeleteSurface() = 0;

	// 幅取得
	virtual Int32 GetWidth() = 0;

	// 高さ取得
	virtual Int32 GetHeight() = 0;
};
