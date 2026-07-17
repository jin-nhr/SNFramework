#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGraphicsResource.h"

// 画像
class SNGUIPicture : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIPicture();

	// デストラクタ
	virtual ~SNGUIPicture();

	// ビットマップ設定
	virtual Void SetBitmap(SNBitmap* bmp);

	// スケーリング設定
	virtual Void SetScaling(SNScalingType scale);

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画前処理
	virtual Void OnPreDraw();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	// 描画後処理
	virtual Void OnPostDraw();

	SNBitmap* Bitmap;
	SNScalingType Scaling;
};
