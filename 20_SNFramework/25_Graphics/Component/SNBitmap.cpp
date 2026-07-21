#include "SNBitmap.h"
#include "SNWindowsAPI.h"

SNBitmap::SNBitmap()
{
	D3DTexture = nullptr;
	D2DTargetBitmap = nullptr;
	D2DSourceBitmap = nullptr;
	return;
}

SNBitmap::~SNBitmap()
{
	DeleteBitmap();

	return;
}


// ビットマップ設定
Void SNBitmap::SetBitmap(Handle d3d_texture, Handle d2d_target, Handle d2d_source)
{
	DeleteBitmap();

	D3DTexture = d3d_texture;
	D2DTargetBitmap = d2d_target;
	D2DSourceBitmap = d2d_source;

	return;
}

// 3Dテクスチャ取得
Handle SNBitmap::Get3DTexture()
{
	return D3DTexture;
}

// ターゲット取得
Handle SNBitmap::GetTargetBitmap()
{
	return D2DTargetBitmap;
}

// ソース取得
Handle SNBitmap::GetSourceBitmap()
{
	return D2DSourceBitmap;
}

// サイズ取得
// とりあえずターゲット側のサイズを取得
Void SNBitmap::GetSize(SNSize* size)
{
	ID2D1Bitmap1* bitmap = (ID2D1Bitmap1*)D2DTargetBitmap;
	D2D1_SIZE_U sizepx = bitmap->GetPixelSize();

	size->Width = sizepx.width;
	size->Height = sizepx.height;

	return;
}

// ビットマップ解放
Void SNBitmap::DeleteBitmap()
{
	ID2D1Bitmap1* btarget = (ID2D1Bitmap1*)D2DTargetBitmap;
	ID2D1Bitmap1* source = (ID2D1Bitmap1*)D2DSourceBitmap;
	ID3D11Texture2D* texture = (ID3D11Texture2D*)D3DTexture;

	if (D2DTargetBitmap != nullptr)
	{
		btarget->Release();
		D2DTargetBitmap = nullptr;
	}

	if (D2DSourceBitmap != nullptr)
	{
		source->Release();
		D2DSourceBitmap = nullptr;
	}

	if (D3DTexture != nullptr)
	{
		texture->Release();
		D3DTexture = nullptr;
	}

	return;
}

