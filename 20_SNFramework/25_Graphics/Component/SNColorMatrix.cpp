#include "SNColorMatrix.h"
#include "SNWindowsAPI.h"

SNColorMatrix::SNColorMatrix()
{
	ColorMatrix = nullptr;
	Matrix5x4F.Allocate(sizeof(D2D1_MATRIX_5X4_F));
	Matrix5x4F.Clear();
	return;
}

SNColorMatrix::~SNColorMatrix()
{
	DeleteColorMatrix();
	Matrix5x4F.Free();
	return;
}


// マトリクス設定
Void SNColorMatrix::SetColorMatrix(Handle color_matrix)
{
	DeleteColorMatrix();

	ColorMatrix = color_matrix;

	return;
}


// マトリクス解放
Void SNColorMatrix::DeleteColorMatrix()
{
	if (ColorMatrix != nullptr)
	{
		((ID2D1Effect*)ColorMatrix)->Release();
		ColorMatrix = nullptr;
	}

	return;
}

// ソース設定
Void SNColorMatrix::SetSource(SNBitmap* source)
{
	ID2D1Effect* effect = (ID2D1Effect*)ColorMatrix;

	if (effect != nullptr)
	{
		// 入力画像をセット
		effect->SetInput(0, (ID2D1Bitmap*)source);
	}

	return;
}

// 行列初期化
Void SNColorMatrix::ClearMatrix()
{
	D2D1_MATRIX_5X4_F* mtx = (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress();

	*mtx =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	return;
}

// 行列設定
Void SNColorMatrix::SetMatrix()
{
	ID2D1Effect* effect = (ID2D1Effect*)ColorMatrix;

	if (effect != nullptr)
	{
		effect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress());
	}

	return;
}

// 乗算
Void SNColorMatrix::Multiply(SNColor* color)
{
	D2D1_MATRIX_5X4_F* combi_mtx = (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress();
	D2D1_MATRIX_5X4_F mat =
	{
		color->Red / 255.0f, 0.0f,                  0.0f,                 0.0f,
		0.0f,                color->Green / 255.0f, 0.0f,                 0.0f,
		0.0f,                0.0f,                  color->Blue / 255.0f, 0.0f,
		0.0f,                0.0f,                  0.0f,                 color->Alpha / 255.0f,	
		0.0f,                0.0f,                  0.0f,                 0.0f
	};

	// 行列合成
	CombineMatrix(combi_mtx, &mat, combi_mtx);

	return;
}


// 加算
Void SNColorMatrix::Add(SNColor* color)
{
	D2D1_MATRIX_5X4_F* combi_mtx = (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress();
	D2D1_MATRIX_5X4_F mat =
	{
		1.0f,                0.0f,                  0.0f,                 0.0f,
		0.0f,                1.0f,                  0.0f,                 0.0f,
		0.0f,                0.0f,                  1.0f,                 0.0f,
		0.0f,                0.0f,                  0.0f,                 1.0f,
		color->Red / 255.0f, color->Green / 255.0f, color->Blue / 255.0f, color->Alpha / 255.0f
	};

	// 行列合成
	CombineMatrix(combi_mtx, &mat, combi_mtx);

	return;
}

// 入力重み付け変換
Void SNColorMatrix::WeightedInput(SNColor* color)
{
	D2D1_MATRIX_5X4_F* combi_mtx = (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress();
	D2D1_MATRIX_5X4_F mat =
	{
		color->Red / 255.0f,   color->Green / 255.0f, color->Blue / 255.0f,  0.0f,
		color->Red / 255.0f,   color->Green / 255.0f, color->Blue / 255.0f,  0.0f,
		color->Red / 255.0f,   color->Green / 255.0f, color->Blue / 255.0f,  0.0f,
		0.0f,                  0.0f,                  0.0f,                  color->Alpha / 255.0f,
		0.0f,                  0.0f,                  0.0f,                  0.0f
	};

	// 行列合成
	CombineMatrix(combi_mtx, &mat, combi_mtx);

	return;
}

// 出力重み付け変換
Void SNColorMatrix::WeightedOutput(SNColor* color)
{
	D2D1_MATRIX_5X4_F* combi_mtx = (D2D1_MATRIX_5X4_F*)Matrix5x4F.GetAddress();
	D2D1_MATRIX_5X4_F mat =
	{
		color->Red / 255.0f,   color->Red / 255.0f,   color->Red / 255.0f,   0.0f,
		color->Green / 255.0f, color->Green / 255.0f, color->Green / 255.0f, 0.0f,
		color->Blue / 255.0f,  color->Blue / 255.0f,  color->Blue / 255.0f,  0.0f,
		0.0f,                  0.0f,                  0.0f,                  color->Alpha / 255.0f,
		0.0f,                  0.0f,                  0.0f,                  0.0f
	};

	// 行列合成
	CombineMatrix(combi_mtx, &mat, combi_mtx);

	return;
}

// 行列合成
Void SNColorMatrix::CombineMatrix(const Void* a_mat, const Void* b_mat, Void* out_mat)
{
	const D2D1_MATRIX_5X4_F* a = (const D2D1_MATRIX_5X4_F*)a_mat;
	const D2D1_MATRIX_5X4_F* b = (const D2D1_MATRIX_5X4_F*)b_mat;
	D2D1_MATRIX_5X4_F* out = (D2D1_MATRIX_5X4_F*)out_mat;

	// 中間バッファ（必須）
	D2D1_MATRIX_5X4_F r;

	// RGB (1〜3行目)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			r.m[i][j] =
				b->m[i][0] * a->m[0][j] +
				b->m[i][1] * a->m[1][j] +
				b->m[i][2] * a->m[2][j] +
				b->m[i][3] * a->m[3][j];
		}
	}

	// Alpha (4行目)
	for (int j = 0; j < 4; j++)
	{
		r.m[3][j] =
			b->m[3][0] * a->m[0][j] +
			b->m[3][1] * a->m[1][j] +
			b->m[3][2] * a->m[2][j] +
			b->m[3][3] * a->m[3][j];
	}

	// Offset (5行目)
	for (int j = 0; j < 4; j++)
	{
		r.m[4][j] = a->m[4][j] + b->m[4][j];
	}

	// out に書き戻す（a と out が同じでも安全）
	*out = r;

	return;
}
