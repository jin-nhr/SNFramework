#include "SNMath.h"



// 算術処理クラス
// 静的メソッドのみの実装とする


// 大きい値を選択
Int64 SNMath::SelectMax(Int64 value1, Int64 value2)
{
	return (value1 > value2) ? value1 : value2;
}

// 小さい値を選択
Int64 SNMath::SelectMin(Int64 value1, Int64 value2)
{
	return (value1 < value2) ? value1 : value2;
}

// サイクリックインクリメント
// 戻り値：min～max
Int64 SNMath::Increment(Int64 value, Int64 min, Int64 max)
{
	Int64 ret_value;

	// 最大値以上なら最小値に戻す
	if (value >= max)
	{
		ret_value = min;
	}

	// 最小値未満だったら最小値にする
	else if (value < min)
	{
		ret_value = min;
	}

	// 範囲内ならインクリメントする
	else
	{
		ret_value = value + 1;
	}

	return ret_value;
}

// サイクリックデクリメント
// 戻り値：min～max
Int64 SNMath::Decrement(Int64 value, Int64 min, Int64 max)
{
	Int64 ret_value;

	// 最小値以下なら最大値にする
	if (value <= min)
	{
		ret_value = max;
	}

	// 最大値を超過してたら最大値にする
	else if (value > max)
	{
		ret_value = max;
	}

	// 範囲内ならデクリメントする
	else
	{
		ret_value = value - 1;
	}

	return ret_value;
}

// 飽和インクリメント
Int64 SNMath::SaturateInc(Int64 value, Int64 max)
{
	Int64 ret_value;

	if (value < max)
	{
		ret_value = value + 1;
	}
	else
	{
		ret_value = max;
	}

	return ret_value;
}

// 飽和デクリメント
Int64 SNMath::SaturateDec(Int64 value, Int64 min)
{
	Int64 ret_value;

	if (value > min)
	{
		ret_value = value - 1;
	}
	else
	{
		ret_value = min;
	}

	return ret_value;
}
