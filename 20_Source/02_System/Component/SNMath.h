#pragma once
#include "SNFrameworkInternal.h"


// 算術処理クラス
// 静的メソッドのみの実装とする

class SNMath
{
public:
	// 三角関数の初期化
	static Void InitTrigFunc();

	static Int RoundToInt(Float64 x);

	// 大きい値を選択
	static Int64 SelectMax(Int64 value1, Int64 value2);

	// 小さい値を選択
	static Int64 SelectMin(Int64 value1, Int64 value2);

	// 
	static Int64 Saturate(Int64 value, Int64 min, Int64 max);

	// 大きい値を選択
	static Float32 SelectMaxF(Float32 value1, Float32 value2);

	// 小さい値を選択
	static Float32 SelectMinF(Float32 value1, Float32 value2);

	// 
	static Float32 SaturateF(Float32 value, Float32 min, Float32 max);

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

	// 距離計算(平方根を求めない点に注意)
	static Int64 CalcDist2(SNPoint* pnt1, SNPoint* pnt2);

	// 方位計算
	static Int8 CalcAngle(SNPoint* pnt1, SNPoint* pnt2);

private:
	static constexpr Float64 SNTrigFuncBitDepth = 127.0;
	static constexpr Int32 SNTrigFuncMaxNum = 255;
	static constexpr Int32 SNTrigFuncOffset = 127;
	static constexpr Int32 SNTrigFuncMax = 127;
	static constexpr Int32 SNTrigFuncMin = -127;
	static constexpr Int32 SNTrigFuncTanMax = 32767;
	static constexpr Int32 SNTrigFuncTanMin = -32767;
	static constexpr Int32 SNTrigFuncATanNum = 12263;
	static constexpr Int32 SNTrigFuncATanOffset = 6131;
	static constexpr Int32 SNTrigFuncATanMax = 6131;
	static constexpr Int32 SNTrigFuncATanMin = -6131;

	static Int8 Sin[SNTrigFuncMaxNum];
	static Int8 Cos[SNTrigFuncMaxNum];
	static Int16 Tan[SNTrigFuncMaxNum];

	static Int8 ArcSin[SNTrigFuncMaxNum];
	static Int8 ArcCos[SNTrigFuncMaxNum];
	static Int8 ArcTan[SNTrigFuncATanNum];
};
