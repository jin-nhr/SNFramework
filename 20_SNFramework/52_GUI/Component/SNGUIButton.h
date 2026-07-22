#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUIText.h"
#include "SNGUI.h"

// 画像
class SNGUIButton : public SNLayerController
{
public:
	// コンストラクタ
	SNGUIButton();

	// デストラクタ
	virtual ~SNGUIButton();

	
	Boolean Hidden;
	Boolean Disable;
	Boolean Push;
	Boolean Selected;

	SNGUIText Caption;

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	virtual Void OnCycle();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	// ステータス判定
	virtual SNGUI::ButtonBlockStatus JudgeStatus();
};
