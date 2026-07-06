#pragma once
#include "SNFrameworkInternal.h"


// デバイスコンテキストクラス

class SNDeviceContext
{
public:
	// コンストラクタ
	SNDeviceContext();

	// デストラクタ
	virtual ~SNDeviceContext();

	// DC生成
	virtual Void CreateDeviceContext();

	// DC破棄
	virtual Void DeleteDeviceContext();

	// DC取得
	virtual Handle GetDC();

	// ビットマップ選択
	virtual Void SetBitmap(Handle bitmap);

	// ビットマップ選択解除
	virtual Void ReleaseBitmap();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DeviceContextからの転送処理

	// 矩形転送
	virtual Void BitBlt(
		Int32 dst_x, Int32 dst_y, 
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height);

	// AND転送
	virtual Void AndBlt(
		Int32 dst_x, Int32 dst_y,
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height);

	// 反転転送
	virtual Void InvertBlt(
		Int32 dst_x, Int32 dst_y,
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height);

	// 矩形転送(マスクあり)
	virtual Void MaskBlt(
		Int32 dst_x, Int32 dst_y, 
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height,
		Handle mask_dc, Int32 mask_x, Int32 mask_y);

	// 拡大縮小転送
	virtual Void StretchBlt(
		Int32 dst_x, Int32 dst_y, Int32 dst_width, Int32 dst_height, 
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_width, Int32 src_height);

	// 抜き色転送
	virtual Void TransparentBlt(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, SNColor* rgb);

	// アルファブレンド
	virtual Void AlphaBlend(Int32 x, Int32 y, Int32 w, Int32 h, Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h, UInt8 alpha, Boolean alpha_ch);

	// クリア
	virtual Void Clear();

	// クリアホワイト
	virtual Void ClearWhite();

public:
	Handle DeviceContext;	// デバイスコンテキスト
	Handle InitialBitmap;	// 初期ビットマップ
	Int32 Width;			// ビットマップ幅
	Int32 Height;			// ビットマップ高さ
};
