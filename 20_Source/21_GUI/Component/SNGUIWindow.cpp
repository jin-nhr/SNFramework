#include "SNGUIWindow.h"
#include "SNGraphicsResManager.h"
#include "SNGraphicsResource.h"
#include "SNBitmap.h"
#include "SNGUI.h"
#include "SNMath.h"

// コンストラクタ
SNGUIWindow::SNGUIWindow()
{
	return;
}

// デストラクタ
SNGUIWindow::~SNGUIWindow()
{
	return;
}

// 初期化
Void SNGUIWindow::OnInitialize()
{
	SNGraphicsResManager::AccessGet(SNGraphicsResWindow);
	return;
}

// 終了処理
Void SNGUIWindow::OnTerminate()
{
	SNGraphicsResManager::AccessRelease(SNGraphicsResWindow);
	return;
}

// 描画処理
Void SNGUIWindow::OnDraw(SNGraphicsContext* grc)
{
	SNRect rect = CalcGlobalRect();

	SNGUI::Tiling9(grc, &rect, SNGraphicsResWindow, SNGUI::GUIBlockDef, &SNGUI::WindowBlockOffset[0]);

	return;
}
