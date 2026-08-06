#include "SNGUI.h"
#include "SNSystemConfig.h"
#include "SNMath.h"
#include "SNGraphicsResManager.h"
#include "SNScene.h"
#include "SNGraphicsContext.h"

// GUIクラス
Void SNGUI::Tiling(SNGraphicsContext* grc, SNRect* dst_rect, SNBitmap* src_bmp, SNRect* src_rect)
{
    Int32 block = SNSystemConfig::GUIBlockSize;
    Int32 remain_w = dst_rect->Width;
    Int32 remain_h = dst_rect->Height;
    Int32 x = dst_rect->PointX;
    Int32 y = dst_rect->PointY;

    SNRect target_rect;
    SNRect draw_src_rect;

    target_rect = *dst_rect;
    draw_src_rect = *src_rect;

    while (remain_h > 0)
    {
        remain_w = dst_rect->Width;
        x = dst_rect->PointX;

        while (remain_w > 0)
        {
            target_rect.PointX = x;
            target_rect.PointY = y;
            target_rect.Width = (Int32)SNMath::SelectMin(block, remain_w);
            target_rect.Height = (Int32)SNMath::SelectMin(block, remain_h);

            draw_src_rect.Width = target_rect.Width;
            draw_src_rect.Height = target_rect.Height;

            grc->DrawImage(&target_rect, src_bmp, &draw_src_rect, SNAlphaMax);

            x += target_rect.Width;
            remain_w -= target_rect.Width;
        }

        y += target_rect.Height;
        remain_h -= target_rect.Height;
    }

    return;
}

Void SNGUI::Tiling9(SNGraphicsContext* grc, SNRect* rect, SNGraphicsResID res_id, const SNPoint* blockdef, const SNPoint* offset)
{
    SNBitmap* win_img = nullptr;
    SNRect dst_rect;
    SNRect src_rect;
    SNRect tiling_rect = { 0 };
    GUIBlockIndex index;
    Int32 block = SNSystemConfig::GUIBlockSize;

    win_img = SNGraphicsResManager::GetResource(res_id);

    src_rect.PointX = 0;
    src_rect.PointY = 0;
    src_rect.Width = block;
    src_rect.Height = block;

    dst_rect = *rect;

    // 設定がどんなに小さくても描画範囲はコーナー分を確保しておく
    dst_rect.Width = (Int32)SNMath::SelectMax(dst_rect.Width, block * 2);
    dst_rect.Height = (Int32)SNMath::SelectMax(dst_rect.Height, block * 2);


    ////////////////////////////////////////////
    // 4隅の共通設定
    tiling_rect.Width = block;
    tiling_rect.Height = block;

    // 左上
    index = GUIBlockLeftUp;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右上
    index = GUIBlockRightUp;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 左下
    index = GUIBlockLeftBottom;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右下
    index = GUIBlockRightBottom;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    ////////////////////////////////////////////
    // 上下共通
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = block;

    // 上
    index = GUIBlockUp;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 下
    index = GUIBlockBottom;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);


    ////////////////////////////////////////////
    // 左右共通
    tiling_rect.Width = block;
    tiling_rect.Height = dst_rect.Height - block * 2;

    // 左
    index = GUIBlockLeft;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    // 右
    index = GUIBlockRight;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + block;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    ////////////////////////////////////////////
    // 中央
    index = GUIBlockCenter;
    src_rect.PointX = blockdef[index].X + offset->X;
    src_rect.PointY = blockdef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + block;
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = dst_rect.Height - block * 2;
    SNGUI::Tiling(grc, &tiling_rect, win_img, &src_rect);

    return;
}



// 初期化処理
Void SNGUI::Initialize()
{
	return;
}

// 起動準備
Void SNGUI::Startup()
{
	return;
}

// 実行
Void SNGUI::Run()
{
	return;
}

// 終了前処理
Void SNGUI::BeforeTerminate()
{
	return;
}

// 終了
Void SNGUI::Terminate()
{
	return;
}
