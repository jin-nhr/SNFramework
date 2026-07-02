#include "SNMemory.h"
#include "SNWindowsAPI.h"

// メモリクラス

// コンストラクタ
SNMemory::SNMemory()
{
	// 変数を初期化
	Size = 0;
	AllocSize = 0;
	Address = nullptr;

	return;
}

// デストラクタ
SNMemory::~SNMemory()
{
	// メモリ確保済みなら解放する
	if (Address != nullptr)
	{
		delete[] Address;
		Address = nullptr;
	}

	return;
}

// メモリ確保
// パラメータ：確保サイズ
Void SNMemory::Allocate(UInt32 size)
{
	UInt32 alloc_size;

	// 0以下を指定されても1確保
	if (size <= 0)
	{
		alloc_size = 1;
	}
	else
	{
		alloc_size = size;
	}

	// サイズを境界サイズの倍数にする
	alloc_size = ((alloc_size + MemoryAlignSize - 1) / MemoryAlignSize) * MemoryAlignSize;

	// 確保サイズで足りないときは再確保する
	if (AllocSize < alloc_size)
	{
		// 事前にFreeを実行しておく
		Free();

		// サイズ分だけメモリ確保
		Address = new UInt8[alloc_size];

		// 確保サイズを更新
		AllocSize = alloc_size;
	}

	// データサイズには要求サイズを設定する
	Size = size;

	return;
}

// メモリ解放
Void SNMemory::Free()
{
	// メモリ確保済みなら解放する
	if (Address != nullptr)
	{
		delete[] Address;
		Address = nullptr;
		Size = 0;
		AllocSize = 0;
	}

	return;
}

// アドレス取得
Void* SNMemory::GetAddress()
{
	return Address;
}

// サイズ取得
UInt32 SNMemory::GetSize()
{
	return Size;
}

// 確保サイズ取得
UInt32 SNMemory::GetAllocSize()
{
	return AllocSize;
}

// メモリクリア
Void SNMemory::Clear()
{
	if (Address != nullptr)
	{
		ZeroMemory(Address, Size);
	}

	return;
}

// メモリコピー
Void SNMemory::Copy(Void* source, UInt32 size)
{
	UInt32 copy_size;

	// 配列サイズをコピーサイズ上限にする
	if (Size < size)
	{
		copy_size = Size;
	}
	else
	{
		copy_size = size;
	}

	if (Address != nullptr)
	{
		CopyMemory(Address, source, copy_size);
	}

	return;
}
