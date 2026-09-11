#include "SNBitmap.h"
#include "SNWindowsAPI.h"

SNBitmap::SNBitmap()
{
	D3DTexture = nullptr;
	D3DSrv = nullptr;
	D3DRtv = nullptr;
	return;
}

SNBitmap::~SNBitmap()
{
	DeleteBitmap();

	return;
}


// ビットマップ設定
Void SNBitmap::SetBitmap(Handle d3d_texture, Handle srv, Handle rtv)
{
	DeleteBitmap();

	D3DTexture = d3d_texture;
	D3DSrv = srv;
	D3DRtv = rtv;

	return;
}

// 3Dテクスチャ取得
Handle SNBitmap::Get3DTexture()
{
	return D3DTexture;
}

Handle SNBitmap::GetSRV()
{
	return D3DSrv;
}

Handle SNBitmap::GetRTV()
{
	return D3DRtv;
}

// サイズ取得
// とりあえずターゲット側のサイズを取得
Void SNBitmap::GetSize(SNSize* size)
{
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* texture = (ID3D11Texture2D*)D3DTexture;

	texture->GetDesc(&desc);

	size->Width = desc.Width;
	size->Height = desc.Height;

	return;
}

// ビットマップ解放
Void SNBitmap::DeleteBitmap()
{
	ID3D11Texture2D* texture = (ID3D11Texture2D*)D3DTexture;
	ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)D3DSrv;
	ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)D3DRtv;

	if (D3DRtv != nullptr)
	{
		rtv->Release();
		D3DRtv = nullptr;
	}

	if (D3DSrv != nullptr)
	{
		srv->Release();
		D3DSrv = nullptr;
	}

	if (D3DTexture != nullptr)
	{
		texture->Release();
		D3DTexture = nullptr;
	}

	return;
}

