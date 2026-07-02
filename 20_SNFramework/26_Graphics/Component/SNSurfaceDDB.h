#pragma once
#include "SNFrameworkInternal.h"
#include "SNDeviceContext.h"
#include "SNDDB.h"
#include "SNSurface.h"

// サーフェスクラス(DDB)
class SNSurfaceDDB : public SNSurface, public SNDDB
{
public:
	// コンストラクタ
	SNSurfaceDDB();

	// デストラクタ
	virtual ~SNSurfaceDDB();

	// サーフェス生成
	Void CreateSurface(Int32 width, Int32 height) override;

	// サーフェス破棄
	Void DeleteSurface() override;

	// 幅取得
	Int32 GetWidth();

	// 高さ取得
	Int32 GetHeight();
};
