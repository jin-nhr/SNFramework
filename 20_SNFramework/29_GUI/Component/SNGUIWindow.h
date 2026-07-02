#pragma once
#include "SNFramework.h"
#include "SNScene.h"

// 矩形クラス
class SNGUIRect : public virtual SNScene
{
public:
	static const UInt8 SNRectAlphaMax = 255;
	static const UInt8 SNRectAlphaMin = 0;

public:
	// コンストラクタ
	SNGUIRect();

	// デストラクタz
	virtual ~SNGUIRect();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 色設定
	Void SetColor(SNColorCode Color);

	// アルファ設定
	Void SetAlpha(UInt8 alpha);

protected:
	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	// 色
	SNColorCode Color;

	// Alpha
	UInt8 Alpha;
};

