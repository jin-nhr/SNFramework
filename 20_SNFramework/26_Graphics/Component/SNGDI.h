#pragma once
#include "SNFrameworkInternal.h"
#include "SNAutoResource.h"
#include "SNCriticalSection.h"

// GDI
// RAIIにてGDIリソースのロック/アンロックを制御する
// このクラスのインスタンスを介してGDIのAPIにアクセスすることで
// マルチスレッド環境においての動作を保証する
class SNGDI : public SNAutoResource
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

private:
	// エンコーダー取得
	static Int GetEncoderClsid(String format, Void* clsid);

	// GDIロック用クリティカルセクション
	static SNCriticalSection CriticalSectionForGDILock;

	// GDI+
	static Void* GDIPlusToken;

	// PNGエンコーダーCLSID実体
	static UInt8 EncoderPNG_data[16];

	// PNGエンコーダーポインタ
	static Void* EncoderPNG;


public:
	// コンストラクタ
	SNGDI();

	// デストラクタ
	virtual ~SNGDI();


	///////////////////////////////////////////////////
	// GDI APIラッパー

	// イメージファイルのロード
	Handle LoadImageFile(String file_full);

	// イメージファイルのセーブ
	Boolean SaveImageFile(String file_full, Handle image);

	// DC初期化
	Void InitDC(Handle dc);

	// メモリDC生成
	Handle CreateMemoryDC(Handle base_dc);

	// DC削除
	Void DeleteDC(Handle dc);

	// DDB生成
	Handle CreateBitmap(Handle dc, Int32 w, Int32 h);

	// DIBSection生成
	Handle CreateDIBSection(Handle dc, SNBitmapInfo* bmi, Void** pixel_data);

	// ビットマップ選択
	// もとの選択ビットマップを返す
	Handle SetBitmap(Handle dc, Handle bitmap);

	// ビットマップのサイズ取得
	Void GetBitmapSize(Handle bitmap, SNSize* size);

	// 塗りつぶし
	Void PatBlt(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, UInt32 rop);

	// 矩形転送
	Void BitBlt(
		Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 w, Int32 h, 
		Handle src_dc, Int32 src_x, Int32 src_y, 
		UInt32 rop);

	// 拡大縮小転送
	Void StretchBlt(
		Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h,
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h,
		UInt32 rop);

	// 透過転送
	Void TransparentBlt(
		Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h,
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h,
		SNColor* rgb);

	// アルファブレンド
	Void AlphaBlend(
		Handle dst_dc, Int32 dst_x, Int32 dst_y, Int32 dst_w, Int32 dst_h,
		Handle src_dc, Int32 src_x, Int32 src_y, Int32 src_w, Int32 src_h,
		UInt8 alpha, Boolean alpha_ch);

	// ブラシ生成
	Handle CreateBrush(SNColor* color);

	// ブラシでの塗りつぶし
	Void Fill(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, Handle brush);

	// ペン生成
	Handle CreatePen(Int32 style, Int32 width, SNColor* color);

	// ライン描画
	Void DrawLine(Handle dc, SNPoint* point, UInt8 point_num, Handle pen);

	// オブジェクト削除
	Void DeleteObject(Handle gdi_obj);

	// テキスト描画
	Void DrawString(Handle dc, Int32 x, Int32 y, String str, Int32 len, SNColor* color);

	// 円描画
	Void DrawCircle(Handle dc, Int32 x, Int32 y, Int32 w, Int32 h, Handle brush, Handle pen);
};

