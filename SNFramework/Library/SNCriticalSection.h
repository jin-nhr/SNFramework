#pragma once
#include "../Include/SNFramework.h"

// クリティカルセクション
class SNCriticalSection
{
public:
	// コンストラクタ
	SNCriticalSection();

	// デストラクタ
	virtual ~SNCriticalSection();

	// 初期化
	Void Initialize();

	// ロック
	Void Lock();

	// アンロック
	Void Unlock();

private:
	Handle CriticalSection;
};

