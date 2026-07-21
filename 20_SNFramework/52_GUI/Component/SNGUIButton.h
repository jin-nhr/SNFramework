#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"

// 画像
class SNGUIButton : public SNLayerController
{
public:
	// コンストラクタ
	SNGUIButton();

	// デストラクタ
	virtual ~SNGUIButton();

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

};
