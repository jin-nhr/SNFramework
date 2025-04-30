#include "SNArithmetic.h"



// 算術処理クラス
// 静的メソッドのみの実装とする

 // サイクリックインクリメント
Int32 SNArithmetic::CyclicIncrement(Int32 index, Int32 min, Int32 max)
{
	Int32 ret_index;

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
Int32 SNArithmetic::CyclicDecrement(Int32 index, Int32 min, Int32 max)
{
	Int32 ret_index;

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

// コンストラクタ
SNArithmetic::SNArithmetic()
{
	return;
}
