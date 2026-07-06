#pragma once
#include "SNFrameworkInternal.h"
#include "SNSurface.h"
#include "SNResourceIF.h"

// サーフェスクラス
class SNSurfaceD3D : public SNSurface
{
public:
	// コンストラクタ
	SNSurfaceD3D();

	// デストラクタ
	virtual ~SNSurfaceD3D();

	// サーフェス生成
	virtual Void CreateSurface(Int32 width, Int32 height);

	// サーフェス破棄
	virtual Void DeleteSurface();

	// 幅取得
	virtual Int32 GetWidth();

	// 高さ取得
	virtual Int32 GetHeight();


	// DC生成
	virtual Void CreateDeviceContext();

	// DC破棄
	virtual Void DeleteDeviceContext();

	// ビットマップ選択
	virtual Void SetBitmap(Handle bitmap);

	// ビットマップ選択解除
	virtual Void ReleaseBitmap();
};
