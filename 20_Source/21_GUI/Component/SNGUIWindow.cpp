#include "SNGUIWindow.h"
#include "SNGraphicsResManager.h"
#include "SNGraphicsResource.h"
#include "SNBitmap.h"
#include "SNGUI.h"
#include "SNGUIDef.h"
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
	SNGraphicsResManager::AccessGet(SNGraphicsResGUIAndWorld);
	return;
}

// 終了処理
Void SNGUIWindow::OnTerminate()
{
	SNGraphicsResManager::AccessRelease(SNGraphicsResGUIAndWorld);
	return;
}

// 描画処理
Void SNGUIWindow::OnDraw()
{
	SNRect rect = CalcGlobalRect();

	SNGUI::Tiling9(&rect, SNGraphicsResGUIAndWorld, SNGUIDef::GUIWindowBlockDef, &SNGUIDef::WindowBlockOffset[0]);

	return;
}
