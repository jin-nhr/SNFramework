#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"

class SNBitmap
{
public:
	SNBitmap();
	
	virtual ~SNBitmap();

	// ビットマップ設定
	virtual Void SetBitmap(Handle d3d_texture, Handle srv, Handle rtv);

	virtual Handle Get3DTexture();

	virtual Handle GetSRV();

	virtual Handle GetRTV();

	// サイズ取得
	virtual Void GetSize(SNSize* size);

	// ビットマップ解放
	virtual Void DeleteBitmap();

private:
	Handle D3DTexture;
	Handle D3DSrv;
	Handle D3DRtv;
};
