#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"


// GUI画像クラス
class SNGUIImage : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIImage();

	// デストラクタ
	virtual ~SNGUIImage();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// サーフェス設定
	Void SetSurface(SNSurface* surface);

	// アルファ設定
	Void SetAlpha(UInt8 Alpha);

protected:
	// 描画範囲調整
	Void AdjustDrawRect(SNRect* dst, SNRect* src);

	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	SNSurface*	Surface;		// サーフェス
	UInt8		Alpha;			// アルファ
};
