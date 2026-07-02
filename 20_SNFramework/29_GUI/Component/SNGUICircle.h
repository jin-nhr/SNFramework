#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// GUI円クラス
class SNGUICircle : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUICircle();

	// デストラクタ
	virtual ~SNGUICircle();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 色設定
	Void SetColor(SNColorCode Color);

protected:
	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	SNColorCode Color;	// 色
};
