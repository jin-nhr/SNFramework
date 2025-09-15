#include "SNDDB.h"
#include "../System/SNSystem.h"
#include "../System/SNWindowsAPI.h"

// DIBセクションクラス

// コンストラクタ
SNDDB::SNDDB()
{
	// 変数初期化
	DDBHandle = nullptr;
	Width = 0;
	Height = 0;

	return;
}

// デストラクタ
SNDDB::~SNDDB()
{
	// DDB有効なら破棄する
	if (DDBHandle != nullptr)
	{
		DeleteObject((HGDIOBJ)DDBHandle);
		DDBHandle = nullptr;
		Width = 0;
		Height = 0;
	}

	return;
}

// DDB生成
Void SNDDB::CreateDDB(Int32 width, Int32 height)
{
	// 破棄しておく
	DeleteDDB();

	Width = width;
	Height = height;

	// DDB生成
	DDBHandle = (Handle)CreateCompatibleBitmap(
		(HDC)SNWindow::WindowDC,
		width,
		height);

	return;
}

// DDB破棄
Void SNDDB::DeleteDDB()
{
	// DDB有効なら破棄する
	if (DDBHandle != nullptr)
	{
		DeleteObject((HGDIOBJ)DDBHandle);
		DDBHandle = nullptr;
		Width = 0;
		Height = 0;
	}

	return;
}

// ハンドル取得
Handle SNDDB::GetDDB()
{
	return DDBHandle;
}

// 幅取得
Int32 SNDDB::GetWidth()
{
	return Width;
}

// 高さ取得
Int32 SNDDB::GetHeight()
{
	return Height;
}
