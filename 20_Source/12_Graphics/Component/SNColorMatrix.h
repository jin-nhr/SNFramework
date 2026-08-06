#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"
#include "SNBitmap.h"
#include "SNGraphicsContext.h"
#include "SNMemory.h"

class SNColorMatrix
{
public:
	SNColorMatrix();
	
	virtual ~SNColorMatrix();

	// カラーマトリクス設定
	virtual Void SetColorMatrix(Handle color_matrix);

	// カラーマトリクス解放
	virtual Void DeleteColorMatrix();

	// ソース設定
	virtual Void SetSource(SNBitmap* source);

	// 行列クリア
	virtual Void ClearMatrix();

	// 行列設定
	virtual Void SetMatrix();

	// 乗算
	virtual Void Multiply(SNColor* color);

	// 加算
	virtual Void Add(SNColor* color);

	// 入力重み付け
	virtual Void WeightedInput(SNColor* color);

	// 出力重み付け
	virtual Void WeightedOutput(SNColor* color);

	// 行列合成
	virtual Void CombineMatrix(const Void* a_mat, const Void* b_mat, Void* out_mat);

public:
	Handle ColorMatrix;
	SNMemory Matrix5x4F;
};
