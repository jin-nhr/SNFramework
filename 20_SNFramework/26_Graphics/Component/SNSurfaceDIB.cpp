#include "SNSurfaceDIB.h"



// サーフェスクラス(DIB)

// コンストラクタ
SNSurfaceDIB::SNSurfaceDIB() : SNSurface()
{
	return;
}

// デストラクタ
SNSurfaceDIB::~SNSurfaceDIB()
{
	// ビットマップの選択解除
	ReleaseBitmap();

	// DC削除
	DeleteDeviceContext();

	// ビットマップ削除
	DeleteDIBSection();

	return;
}

// サーフェス生成
Void SNSurfaceDIB::CreateSurface(Int32 width, Int32 height)
{
	// ビットマップ生成
	CreateDIBSection(width, height);

	// DC生成
	CreateDeviceContext();

	// ビットマップを選択
	SetBitmap(GetDIBSection());

	return;
}

// サーフェス破棄
Void SNSurfaceDIB::DeleteSurface()
{
	// ビットマップの選択解除
	ReleaseBitmap();

	// DC削除
	DeleteDeviceContext();

	// ビットマップ削除
	DeleteDIBSection();

	return;
}

// 幅取得
Int32 SNSurfaceDIB::GetWidth()
{
	return SNDIBSection::GetWidth();
}

// 高さ取得
Int32 SNSurfaceDIB::GetHeight()
{
	return SNDIBSection::GetHeight();
}
