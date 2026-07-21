#include "SNGUIButton.h"
#include "SNGraphicsResManager.h"
#include "SNGraphicsResource.h"
#include "SNBitmap.h"
#include "SNGUI.h"
#include "SNMath.h"

// コンストラクタ
SNGUIButton::SNGUIButton()
{
    return;
}

// デストラクタ
SNGUIButton::~SNGUIButton()
{
    return;
}

// 初期化
Void SNGUIButton::OnInitialize()
{
    SNGraphicsResManager::AccessGet(SNGraphicsResButton);
    return;
}

// 終了処理
Void SNGUIButton::OnTerminate()
{
    SNGraphicsResManager::AccessRelease(SNGraphicsResButton);
    return;
}


// 描画処理
Void SNGUIButton::OnDraw(SNGraphicsContext* grc)
{
    SNBitmap* btn_img;
    SNRect dst_rect;
    SNRect src_rect;
    SNRect tiling_rect;
    SNGUI::WindowBlockIndex index;
    Int32 block = SNSystemConfig::GUIBlockSize;

    btn_img = SNGraphicsResManager::GetResource(SNGraphicsResButton);

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
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右上
//    index = SNGUI::WindowBlockIndex::RightUp;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 左下
 //   index = SNGUI::WindowBlockIndex::LeftBottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右下
//    index = SNGUI::WindowBlockIndex::RightBottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    ////////////////////////////////////////////
    // 上下共通
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = block;

    // 上
//    index = SNGUI::WindowBlockIndex::Up;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 下
//    index = SNGUI::WindowBlockIndex::Bottom;
    src_rect.PointX = SNGUI::WindowBlockDef[index].X;
    src_rect.PointY = SNGUI::WindowBlockDef[index].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    return;
}
