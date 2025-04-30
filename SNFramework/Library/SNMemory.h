#pragma once
#include "../Include/SNFramework.h"

// メモリクラス
class SNMemory
{
public:
	// コンストラクタ
	SNMemory();

	// デストラクタ
	~SNMemory();

	// メモリ確保
	// パラメータ：確保サイズ
	Void Allocate(UInt32 size);

	// メモリ解放
	Void Free();

	// アドレス取得
	Void* GetAddress();

	// メモリクリア
	Void Clear();

	// メモリコピー
	Void Copy(Void* source, UInt32 size);

private:
	UInt32 Size;	// 確保サイズ
	UInt8* Address;	// メモリアドレス
};
