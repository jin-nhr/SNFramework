#include "SNMath.h"
#include "SNWindowsAPI.h"


// 算術処理クラス
// 静的メソッドのみの実装とする

Int8 SNMath::Sin[SNTrigFuncMaxNum];
Int8 SNMath::Cos[SNTrigFuncMaxNum];
Int16 SNMath::Tan[SNTrigFuncMaxNum];

Int8 SNMath::ArcSin[SNTrigFuncMaxNum];
Int8 SNMath::ArcCos[SNTrigFuncMaxNum];
Int8 SNMath::ArcTan[SNTrigFuncATanNum];



// 三角関数の初期化
Void SNMath::InitTrigFunc()
{
	Int i;
	Int angle;
	Float64 rad;
	Float64 s, c, t;
	Int sinv, cosv, tanv;
	Float64 M_PI = 3.14159265358979323846;
	Float64 slope;

	for (i = 0; i < SNTrigFuncMaxNum; i++)
	{
		// 角度（-127 ～ 127）
		angle = i - SNTrigFuncOffset;

		// ラジアン変換
		rad = (Float64)angle * (M_PI / SNTrigFuncBitDepth);

		// ---- 三角関数 ----
		s = sin(rad);
		c = cos(rad);
		t = tan(rad);

		// ---- 量子化 ----
		sinv = RoundToInt((Float32)(s * SNTrigFuncBitDepth));
		cosv = RoundToInt((Float32)(c * SNTrigFuncBitDepth));
		tanv = RoundToInt((Float32)(t * SNTrigFuncBitDepth));

		// ---- クリップ ----
		sinv = (Int)Saturate(sinv, SNTrigFuncMin, SNTrigFuncMax);
		cosv = (Int)Saturate(cosv, SNTrigFuncMin, SNTrigFuncMax);
		tanv = (Int)Saturate(tanv, SNTrigFuncTanMin, SNTrigFuncTanMax);

		Sin[i] = (Int8)sinv;
		Cos[i] = (Int8)cosv;
		Tan[i] = (Int16)tanv;

		// ---- Arc 系（角度そのまま入れる）----
		ArcSin[i] = (Int8)angle;
		ArcCos[i] = (Int8)angle;
	}

	// atanは別計算
	for (i = 0; i < SNTrigFuncATanNum; i++)
	{
		slope = ((i - SNTrigFuncATanOffset) / SNTrigFuncBitDepth);
		ArcTan[i] = (Int)Saturate(RoundToInt((Float32)(atan(slope) * SNTrigFuncBitDepth / M_PI)), SNTrigFuncMin, SNTrigFuncMax);
	}

	return;
}

Int SNMath::RoundToInt(Float32 x)
{
	return (x >= 0.0) ? (int)(x + 0.5) : (int)(x - 0.5);
}

Int SNMath::FloorToInt(Float32 x)
{
	Int32 i = (Int32)x;   // 0方向への丸め（truncate）
	return (x < (Float32)i) ? (i - 1) : i;
}

// 絶対値取得
Float32 SNMath::AbsF(Float32 x)
{
	Float32 ret = x;

	if (x < 0)
	{
		x = x * -1.0f;
	}

	return x;
}

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

Int64 SNMath::Saturate(Int64 value, Int64 min, Int64 max)
{
	return SelectMax(SelectMin(value, max), min);
}

// 大きい値を選択
Float32 SNMath::SelectMaxF(Float32 value1, Float32 value2)
{
	return (value1 > value2) ? value1 : value2;
}

// 小さい値を選択
Float32 SNMath::SelectMinF(Float32 value1, Float32 value2)
{
	return (value1 < value2) ? value1 : value2;
}

Float32 SNMath::SaturateF(Float32 value, Float32 min, Float32 max)
{
	return SelectMaxF(SelectMinF(value, max), min);
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

Int64 SNMath::CalcDist2(SNPoint* pnt1, SNPoint* pnt2)
{
	Int64 delta_x = pnt2->X - pnt1->X;
	Int64 delta_y = pnt2->Y - pnt1->Y;

	Int64 dist2 = delta_x * delta_x + delta_y * delta_y;

	return dist2;
}

Int8 SNMath::CalcAngle(SNPoint* pnt1, SNPoint* pnt2)
{
	Int dx = pnt2->X - pnt1->X;
	Int dy = pnt2->Y - pnt1->Y;
	Int slope;
	Float64 f;
	Int8 angle;

	if (dx == 0)
	{
		// 垂直方向は tan が無限大
		slope = (dy >= 0) ? SNTrigFuncTanMax : SNTrigFuncTanMin;
	}
	else
	{
		f = (Float64)dy / (Float64)dx;
		slope = (Int)RoundToInt((Float32)(f * SNTrigFuncBitDepth));
		slope = (Int)Saturate(slope, SNTrigFuncTanMin, SNTrigFuncTanMax);
	}

	// 画面上のY座標系はトップダウンなのでSlopeを反転する
	slope = slope * -1;

	// ATanの収容範囲に限定する
	slope = (Int)Saturate(slope, SNTrigFuncATanMin, SNTrigFuncATanMax);

	angle = ArcTan[slope + SNTrigFuncATanOffset];

	// 象限補正
	if (dx < 0)
	{
		if (angle >= 0)
		{
			angle += SNTrigFuncMin;
		}
		else
		{
			angle += SNTrigFuncMax;
		}
	}

	return angle;
}
