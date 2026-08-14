#pragma once
#include "SNFrameworkInternal.h"
#include "SNBitmap.h"


class SNTargetBitmap;
class SNSourceBitmap;
class SNBitmap;
class SNDIB;
class SNDraw;
class SNColorMatrix;
struct SNListContainer;

// グラフィクスコンテキスト

class SNGraphicsContext
{
public:
	static Int32 DrawImageCounter;
	static Int32 DrawImageCounterWork;

public:
	// コンストラクタ
	SNGraphicsContext();

	// デストラクタ
	virtual ~SNGraphicsContext();

	// コンテキスト生成
	virtual Void CreateDeviceContext();

	// コンテキスト削除
	virtual Void DeleteDeviceContext();

	// 描画開始
	virtual Void Begin(SNBitmap* bmp);

	// 描画終了
	virtual Void End();

	// DC取得
	virtual Handle GetDC();

	// ビットマップ生成
	virtual Void CreateBitmap(SNBitmap* bmp, SNSize* size);

	// 矩形描画
	virtual Void DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha);

	// クリア
	virtual Void Clear(SNColor* color);

	// カラーマトリクス変換
	virtual Void ColorMatrixEffect(SNPoint* dst_pnt, SNRect* src_rect, SNColorMatrix* color_matrix);

	// BitmapからDIBを生成する
	virtual Void CreateDIBFromBitmap(SNBitmap* src_bitmap, SNDIB* dst_dib);

	// DIBからBitmapを生成する
	virtual Void CreateBitmapFromDIB(SNDIB* src_dib, SNBitmap* dst_bitmap);

	// カラーマトリクス生成
	virtual Void CreateColorMatrix(SNColorMatrix* color_matrix);

private:
	Handle DeviceContext;

};
