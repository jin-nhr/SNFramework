#pragma once
#include "SNFrameworkInternal.h"


// DIBセクションクラス

class SNDIBSection
{
public:
	// コンストラクタ
	SNDIBSection();

	// デストラクタ
	~SNDIBSection();

	// DIBセクション生成
	Void CreateDIBSection(Int32 width, Int32 height);

	// DIBセクション破棄
	Void DeleteDIBSection();

	// ハンドル取得
	Handle GetDIBSection();
	
	// ピクセルデータ取得
	SNColor* GetPixelData();

	// 幅取得
	Int32 GetWidth();

	// 高さ取得
	Int32 GetHeight();

private:
	Handle DIBSection;			// DIBSectionハンドル
	SNBitmapInfo BitmapInfo;	// ビットマップ情報
	SNColor* PixelData;			// ピクセルデータアドレス
	
};
