#include "SNGUI.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

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
