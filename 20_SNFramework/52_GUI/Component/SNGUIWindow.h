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


	// Entry/Exitは本来オーバーライドを想定していないが
	// Window開閉音を共通で鳴らすためにこのクラスではオーバーライドする
	virtual Void Entry();

	virtual Void Exit();

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	SNSoundEffect SEOpen;
};
