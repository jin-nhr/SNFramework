#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNSoundEffect.h"

// 画像
class SNGUIWindow : public SNLayerController
{
public:
	// コンストラクタ
	SNGUIWindow();

	// デストラクタ
	virtual ~SNGUIWindow();

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);
};
