#pragma once
#include "../Include/SNFramework.h"

// メモリクラス
class SNMemory
{
public:
	static const Int32 SNMemoryAlignSize;	// メモリ境界サイズ

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

	// データサイズ取得
	UInt32 GetSize();

	// 確保サイズ取得
	UInt32 GetAllocSize();

	// メモリクリア
	Void Clear();

	// メモリコピー
	Void Copy(Void* source, UInt32 size);

private:
	UInt32 Size;		// データサイズ
	UInt32 AllocSize;	// 確保サイズ
	UInt8* Address;		// メモリアドレス
};
