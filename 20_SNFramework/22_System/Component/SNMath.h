#pragma once
#include "SNFrameworkInternal.h"


// 算術処理クラス
// 静的メソッドのみの実装とする

class SNMath
{
public:
	// 大きい値を選択
	static Int64 SelectMax(Int64 value1, Int64 value2);

	// 小さい値を選択
	static Int64 SelectMin(Int64 value1, Int64 value2);

	// 大きい値を選択
	static Float32 SelectMaxF(Float32 value1, Float32 value2);

	// 小さい値を選択
	static Float32 SelectMinF(Float32 value1, Float32 value2);

	// サイクリックインクリメント
	// 最大値を超えた場合は最小値に戻る
	static Int64 Increment(Int64 value, Int64 min, Int64 max);

	// サイクリックデクリメント
	// 最小値を下回った場合は最大値に戻る
	static Int64 Decrement(Int64 value, Int64 min, Int64 max);

	// 飽和インクリメント
	static Int64 SaturateInc(Int64 value, Int64 max);

	// 飽和デクリメント
	static Int64 SaturateDec(Int64 value, Int64 min);

};
