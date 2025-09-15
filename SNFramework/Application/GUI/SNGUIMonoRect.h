#pragma once
#include "../../Include/SNFramework.h"
#include "../Controller/SNScene.h"

// 白黒矩形
class SNGUIMonoRect : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIMonoRect();

	// デストラクタ
	virtual ~SNGUIMonoRect();

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 色設定 true:白 false:黒
	Void SetColor(Boolean whiteness);

protected:
	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	Boolean Whiteness;	// 白
};

