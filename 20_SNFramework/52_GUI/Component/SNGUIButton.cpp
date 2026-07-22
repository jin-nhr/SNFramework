#include "SNGUIButton.h"
#include "SNGraphicsResManager.h"
#include "SNGraphicsResource.h"
#include "SNBitmap.h"
#include "SNMath.h"

// コンストラクタ
SNGUIButton::SNGUIButton()
{
    Hidden = false;
    Disable = false;
    Push = false;
    Selected = false;

    // 高さは基本固定
    Size.Height = SNSystemConfig::GUIBlockSize * 2;

    SetSceneNum(1);
    SetScene(&Caption);

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

Void SNGUIButton::OnCycle()
{
    SNGUI::ButtonBlockStatus button_sts = JudgeStatus();

    Caption.Color = SNGUI::FontColor[SNGUI::ButtonCaptionColor[button_sts]];

    return;
}


// 描画処理
Void SNGUIButton::OnDraw(SNGraphicsContext* grc)
{
    SNBitmap* btn_img;
    SNRect dst_rect;
    SNRect src_rect;
    SNRect tiling_rect;
    SNGUI::ButtonBlockIndex index;
    SNGUI::ButtonBlockStatus button_sts = JudgeStatus();
    Int32 block = SNSystemConfig::GUIBlockSize;

    btn_img = SNGraphicsResManager::GetResource(SNGraphicsResButton);

    src_rect.PointX = 0;
    src_rect.PointY = 0;
    src_rect.Width = block;
    src_rect.Height = block;

    dst_rect = CalcGlobalRect();

    // 設定がどんなに小さくても描画範囲はコーナー分を確保しておく
    dst_rect.Width = (Int32)SNMath::SelectMax(dst_rect.Width, block * 2);
    dst_rect.Height = (Int32)SNMath::SelectMax(dst_rect.Height, block * 2);


    ////////////////////////////////////////////
    // 4隅の共通設定
    tiling_rect.Width = block;
    tiling_rect.Height = block;

    // 左上
    index = SNGUI::ButtonBlockLeftUp;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右上
    index = SNGUI::ButtonBlockRightUp;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 左下
    index = SNGUI::ButtonBlockLeftBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右下
    index = SNGUI::ButtonBlockRightBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    ////////////////////////////////////////////
    // 上下共通
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = block;

    // 上
    index = SNGUI::ButtonBlockUp;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 下
    index = SNGUI::ButtonBlockBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + SNGUI::ButtonBlockOffset[button_sts].X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + SNGUI::ButtonBlockOffset[button_sts].Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    return;
}


SNGUI::ButtonBlockStatus SNGUIButton::JudgeStatus()
{
    SNGUI::ButtonBlockStatus ret = SNGUI::ButtonStatusNormal;

    if (Hidden)
    {
        ret = SNGUI::ButtonStatusHidden;
    }

    else if (Disable)
    {
        ret = SNGUI::ButtonStatusDisable;
    }

    else if (Push)
    {
        ret = SNGUI::ButtonStatusPush;
    }

    else if (Selected)
    {
        ret = SNGUI::ButtonStatusSelected;
    }

    return ret;
}

