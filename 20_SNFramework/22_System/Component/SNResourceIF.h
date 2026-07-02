#pragma once
#include "SNFrameworkInternal.h"

// アンマネージリソースクラスのインターフェース
// SNAutoResourceによる自動化に使う
class SNResourceIF
{
public:
	// コンストラクタ
	SNResourceIF();

	// デストラクタ
	virtual ~SNResourceIF();

	// 確保
	virtual Void Acquire() = 0;

	// 解放
	virtual Void Release() = 0;

private:

};

