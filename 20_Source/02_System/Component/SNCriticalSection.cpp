#include "SNCriticalSection.h"
#include "SNWindowsAPI.h"

// クリティカルセクション

// コンストラクタ
SNCriticalSection::SNCriticalSection()
{
	CriticalSection = nullptr;

	return;
}

// デストラクタ
SNCriticalSection::~SNCriticalSection()
{
	if (CriticalSection != nullptr)
	{
		// クリティカルセクション破棄
		DeleteCriticalSection((CRITICAL_SECTION*)CriticalSection);
		delete (CRITICAL_SECTION*)CriticalSection;
		CriticalSection = nullptr;
	}

	return;
}

// 初期化
Void SNCriticalSection::Initialize()
{
	// 生成済みだったら破棄
	if (CriticalSection != nullptr)
	{
		DeleteCriticalSection((CRITICAL_SECTION*)CriticalSection);
		delete (CRITICAL_SECTION*)CriticalSection;
		CriticalSection = nullptr;
	}

	// クリティカルセクションオブジェクト生成
	CriticalSection = (Handle)(new CRITICAL_SECTION);

	// クリティカルセクションオブジェクト初期化
	InitializeCriticalSection((CRITICAL_SECTION*)CriticalSection);

	return;
}

// ロック
Void SNCriticalSection::Lock()
{
	// 生成済みなら処理する
	if (CriticalSection != nullptr)
	{
		// クリティカルセクションへの入場
		EnterCriticalSection((CRITICAL_SECTION*)CriticalSection);
	}

	return;
}

// アンロック
Void SNCriticalSection::Unlock()
{
	// 生成済みなら処理する
	if (CriticalSection != nullptr)
	{
		// クリティカルセクションへの退出
		LeaveCriticalSection((CRITICAL_SECTION*)CriticalSection);
	}

	return;
}

// AutoResource用確保IF
Void SNCriticalSection::Acquire()
{
	Lock();

	return;
}

// AutoResource用解放IF
Void SNCriticalSection::Release()
{
	Unlock();

	return;
}
