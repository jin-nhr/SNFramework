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
Int64 SNMath::Increment(Int64 index, Int64 min, Int64 max)
{
	Int64 ret_index;

	// 最大値以上なら最小値に戻す
	if (index >= max)
	{
		ret_index = min;
	}

	// 最小値未満だったら最小値にする
	else if (index < min)
	{
		ret_index = min;
	}

	// 範囲内ならインクリメントする
	else
	{
		ret_index = index + 1;
	}

	return ret_index;
}

// サイクリックデクリメント
// 戻り値：min～max
Int64 SNMath::Decrement(Int64 index, Int64 min, Int64 max)
{
	Int64 ret_index;

	// 最小値以下なら最大値にする
	if (index <= min)
	{
		ret_index = max;
	}

	// 最大値を超過してたら最大値にする
	else if (index > max)
	{
		ret_index = max;
	}

	// 範囲内ならデクリメントする
	else
	{
		ret_index = index - 1;
	}

	return ret_index;
}
