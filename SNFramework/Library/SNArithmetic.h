#pragma once
#include "../Include/SNFramework.h"


// 算術処理クラス
// 静的メソッドのみの実装とする

class SNArithmetic
{
public:
	// 大きい値を選択
	static Int64 SelectMaximum(Int64 value1, Int64 value2);

	// 小さい値を選択
	static Int64 SelectMinimum(Int64 value1, Int64 value2);

	// サイクリックインクリメント
	static Int64 CyclicIncrement(Int64 index, Int64 min, Int64 max);

	// サイクリックデクリメント
	static Int64 CyclicDecrement(Int64 index, Int64 min, Int64 max);

private:
	// コンストラクタ
	SNArithmetic();
};
