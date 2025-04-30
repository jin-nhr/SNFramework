#pragma once
#include "../Include/SNFramework.h"


// 算術処理クラス
// 静的メソッドのみの実装とする

class SNArithmetic
{
public:
	// サイクリックインクリメント
	static Int32 CyclicIncrement(Int32 index, Int32 min, Int32 max);

	// サイクリックデクリメント
	static Int32 CyclicDecrement(Int32 index, Int32 min, Int32 max);

private:
	// コンストラクタ
	SNArithmetic();
};
