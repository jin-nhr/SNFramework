#pragma once
#include "SNFrameworkInternal.h"
#include "SNImageCodecAccessor.h"
#include "SNBitmap.h"
#include "SNDIBPixel.h"
#include "SNGraphicsContext.h"

// DIBクラス
// SNBitmap, SNDeviceContextと同様のIFをもつが
// 互換性がないため継承はしない

class SNDIB :public SNImageCodecAccessor
{
public:
	// コンストラクタ
	SNDIB();

	// デストラクタ
	virtual ~SNDIB();

	// DIB生成
	virtual Void CreateDIB(SNSize* size);

	// DIB解放
	virtual Void DeleteDIB();

	// ビットマップ取得
	virtual Handle GetBitmap();

	// ピクセル取得
	virtual SNDIBPixel* GetPixel();

	// ピクセル解放
	virtual Void ReleasePixel(SNDIBPixel* pixel);

	// サイズ取得
	virtual Void GetSize(SNSize* size);

	// デコード(非同期実行)
	// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
	virtual Void Decode(SNMemory* in_data);

	// エンコード(非同期実行)
	// エンコード結果を受け取るメモリオブジェクトを指定する
	virtual Void Encode(SNMemory* out_data);

protected:
	virtual SNImageCodecResult OnOperationImageDecode();
	virtual SNImageCodecResult OnOperationImageEncode();

private:
	Handle WICBitmap;
	SNSize ImageSize;
	SNMemory* TargetData;
};
