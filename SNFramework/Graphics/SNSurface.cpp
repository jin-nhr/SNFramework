#include "SNSurface.h"
#include "../System/SNWindowsAPI.h"

// サーフェスクラス

// コンストラクタ
SNSurface::SNSurface()
{
	return;
}

// デストラクタ
SNSurface::~SNSurface()
{
	// デバイスコンテキスト削除
	DeviceContext.DeleteDeviceContext();

	return;
}

// DC取得
SNDeviceContext* SNSurface::GetDC()
{
	return &DeviceContext;
}


// サーフェスクラス(DDB)

// コンストラクタ
SNSurfaceDDB::SNSurfaceDDB() : SNSurface()
{
	return;
}

// デストラクタ
SNSurfaceDDB::~SNSurfaceDDB()
{
	// ビットマップの選択解除
	DeviceContext.ReleaseBitmap();

	// DC削除
	DeviceContext.DeleteDeviceContext();

	// ビットマップ削除
	DDB.DeleteDDB();

	return;
}

// サーフェス生成
Void SNSurfaceDDB::CreateSurface(Int32 width, Int32 height)
{
	// ビットマップ生成
	DDB.CreateDDB(width, height);

	// DC生成
	DeviceContext.CreateDeviceContext();

	// ビットマップを選択
	DeviceContext.SetBitmap(DDB.GetDDB());

	return;
}

// サーフェス破棄
Void SNSurfaceDDB::DeleteSurface()
{
	// ビットマップの選択解除
	DeviceContext.ReleaseBitmap();

	// DC削除
	DeviceContext.DeleteDeviceContext();

	// ビットマップ削除
	DDB.DeleteDDB();

	return;
}

// 幅取得
Int32 SNSurfaceDDB::GetWidth()
{
	return DDB.GetWidth();
}

// 高さ取得
Int32 SNSurfaceDDB::GetHeight()
{
	return DDB.GetHeight();
}




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
	DeviceContext.ReleaseBitmap();

	// DC削除
	DeviceContext.DeleteDeviceContext();

	// ビットマップ削除
	DIBSection.DeleteDIBSection();

	return;
}

// サーフェス生成
Void SNSurfaceDIB::CreateSurface(Int32 width, Int32 height)
{
	// ビットマップ生成
	DIBSection.CreateDIBSection(width, height);

	// DC生成
	DeviceContext.CreateDeviceContext();

	// ビットマップを選択
	DeviceContext.SetBitmap(DIBSection.GetDIBSection());

	return;
}

// サーフェス破棄
Void SNSurfaceDIB::DeleteSurface()
{
	// ビットマップの選択解除
	DeviceContext.ReleaseBitmap();

	// DC削除
	DeviceContext.DeleteDeviceContext();

	// ビットマップ削除
	DIBSection.DeleteDIBSection();

	return;
}

// 幅取得
Int32 SNSurfaceDIB::GetWidth()
{
	return DIBSection.GetWidth();
}

// 高さ取得
Int32 SNSurfaceDIB::GetHeight()
{
	return DIBSection.GetHeight();
}
