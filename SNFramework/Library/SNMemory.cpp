#include "SNMemory.h"
#include "../System/SNWindowsAPI.h"

// メモリクラス


Int32 SNMemory::AlignSize = 4;


// コンストラクタ
SNMemory::SNMemory()
{
	// 変数を初期化
	Size = 0;
	Address = nullptr;
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

	// 事前にFreeを実行しておく
	Free();

	// サイズを4の倍数にする
	Size = ((alloc_size + AlignSize - 1) / AlignSize) * AlignSize;

	// サイズ分だけメモリ確保
	Address = new UInt8[Size];

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
	}

	return;
}

// アドレス取得
Void* SNMemory::GetAddress()
{
	return Address;
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
