#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUIText.h"
#include "SNGUI.h"
#include "SNGraphicsResource.h"


// 画像
class SNGUIButton : public SNLayerController
{
public:
	// コンストラクタ
	SNGUIButton();

	// デストラクタ
	virtual ~SNGUIButton();

	virtual Boolean CheckHover(SNPoint* pnt);
	
	Boolean NoFrame;
	Boolean Disable;
	Boolean Push;
	Boolean Selected;

	Boolean Focus;

	SNGUIText Caption;

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	virtual Void OnCycle();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	// ボタン描画
	virtual Void DrawButton(SNGraphicsContext* grc);

	// フォーカス描画
	virtual Void DrawFocus(SNGraphicsContext* grc);

	virtual Void ButtonTiling(SNGraphicsContext* grc, SNGraphicsResID res_id, const SNPoint* offset);

	// ステータス判定
	virtual SNGUI::ButtonBlockStatus JudgeStatus();

	virtual SNGUI::ButtonFocusStatus JudgeFocusStatus();
};
