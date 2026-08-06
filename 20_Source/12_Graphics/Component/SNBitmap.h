#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"

class SNBitmap
{
public:
	SNBitmap();
	
	virtual ~SNBitmap();

	// ビットマップ設定
	virtual Void SetBitmap(Handle d3d_texture, Handle d2d_target, Handle d2d_source);

	virtual Handle Get3DTexture();

	// ターゲットビットマップ取得
	virtual Handle GetTargetBitmap();

	// ソースビットマップ取得
	virtual Handle GetSourceBitmap();

	// サイズ取得
	virtual Void GetSize(SNSize* size);

	// ビットマップ解放
	virtual Void DeleteBitmap();

private:
	Handle D3DTexture;
	Handle D2DTargetBitmap;
	Handle D2DSourceBitmap;
};
