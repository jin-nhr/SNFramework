#pragma once
#include "SNFrameworkInternal.h"
#include "SNDeviceContext.h"
#include "SNDIBSection.h"
#include "SNSurface.h"

// サーフェスクラス(DIB)
class SNSurfaceDIB : public SNSurface, public SNDIBSection
{
public:
	// コンストラクタ
	SNSurfaceDIB();

	// デストラクタ
	virtual ~SNSurfaceDIB();

	// サーフェス生成
	Void CreateSurface(Int32 width, Int32 height) override;

	// サーフェス破棄
	Void DeleteSurface() override;

	// 幅取得
	Int32 GetWidth();

	// 高さ取得
	Int32 GetHeight();
};
