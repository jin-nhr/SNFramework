#pragma once
#include "SNFrameworkInternal.h"
#include "SNResourceIF.h"

// クリティカルセクション
// スレッド間の排他制御を行う
class SNCriticalSection : public SNResourceIF
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

	// AutoResource用確保IF
	Void Acquire();

	// AutoResource用解放IF
	Void Release();


private:
	Handle CriticalSection;		// クリティカルセクション
};

