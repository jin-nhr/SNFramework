#include "SNSurfaceDDB.h"



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
	ReleaseBitmap();

	// DC削除
	DeleteDeviceContext();

	// ビットマップ削除
	DeleteDDB();

	return;
}

// サーフェス生成
Void SNSurfaceDDB::CreateSurface(Int32 width, Int32 height)
{
	// ビットマップ生成
	CreateDDB(width, height);

	// DC生成
	CreateDeviceContext();

	// ビットマップを選択
	SetBitmap(GetDDB());

	return;
}

// サーフェス破棄
Void SNSurfaceDDB::DeleteSurface()
{
	// ビットマップの選択解除
	ReleaseBitmap();

	// DC削除
	DeleteDeviceContext();

	// ビットマップ削除
	DeleteDDB();

	return;
}

// 幅取得
Int32 SNSurfaceDDB::GetWidth()
{
	return SNDDB::GetWidth();
}

// 高さ取得
Int32 SNSurfaceDDB::GetHeight()
{
	return SNDDB::GetHeight();
}


