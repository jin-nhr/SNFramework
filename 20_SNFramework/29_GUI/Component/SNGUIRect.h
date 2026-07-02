#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// 矩形クラス
class SNGUIRect : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIRect();

	// デストラクタ
	virtual ~SNGUIRect();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 色設定
	Void SetColor(SNColorCode Color);

	// アルファ設定
	Void SetAlpha(UInt8 alpha);

	// サーフェス設定
	Void SetSurface(SNSurface* surface);

protected:
	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	// 色
	UInt8 Alpha;			// アルファ
	SNSurface* Surface;		// サーフェス	
};

