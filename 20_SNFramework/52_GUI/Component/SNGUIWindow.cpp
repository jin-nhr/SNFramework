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
	SNBitmap* win_img;
	SNRect dst_rect;
	SNRect src_rect;
    SNRect tiling_rect;
    SNGUI::WindowBlockIndex index;
    Int32 block = SNSystemConfig::GUIBlockSize;

	win_img = SNGraphicsResManager::GetResource(SNGraphicsResWindow);

     src_rect.PointX = 0;
    src_rect.PointY = 0;
    src_rect.Width = block;
    src_rect.Height = block;
    
    dst_rect = GetRect();

    // 設定がどんなに小さくても描画範囲はコーナー分を確保しておく
    dst_rect.Width = (Int32)SNMath::SelectMax(dst_rect.Width, block * 2);
    dst_rect.Height = (Int32)SNMath::SelectMax(dst_rect.Height, block * 2);


    ////////////////////////////////////////////
    // 4隅の共通設定
    tiling_rect.Width = block;
    tiling_rect.Height = block;

    // 左上
    index = SNGUI::WindowBlockLeftUp;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右上
    index = SNGUI::WindowBlockRightUp;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 左下
    index = SNGUI::WindowBlockLeftBottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右下
    index = SNGUI::WindowBlockRightBottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    ////////////////////////////////////////////
    // 上下共通
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = block;

    // 上
    index = SNGUI::WindowBlockUp;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 下
    index = SNGUI::WindowBlockBottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);


    ////////////////////////////////////////////
    // 左右共通
    tiling_rect.Width = block;
    tiling_rect.Height = dst_rect.Height - block * 2;

    // 左
    index = SNGUI::WindowBlockLeft;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右
    index = SNGUI::WindowBlockRight;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    ////////////////////////////////////////////
    // 中央
    index = SNGUI::WindowBlockCenter;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + block;
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = dst_rect.Height - block * 2;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

	return;
}
