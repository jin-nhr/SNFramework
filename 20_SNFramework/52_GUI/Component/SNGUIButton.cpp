#include "SNGUIButton.h"
#include "SNBitmap.h"
#include "SNMath.h"
#include "SNGraphicsResManager.h"


// コンストラクタ
SNGUIButton::SNGUIButton()
{
    NoFrame = false;
    Disable = false;
    Push = false;
    Selected = false;
    Focus = false;

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

Boolean SNGUIButton::CheckHover(SNPoint* pnt)
{
    Boolean ret = false;
    SNRect rect = CalcGlobalRect();

    if ((rect.PointX <= pnt->X) && (pnt->X < rect.PointX + rect.Width) &&
        (rect.PointY <= pnt->Y) && (pnt->Y < rect.PointY + rect.Height))
    {
        ret = true;
    }
    return ret;
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
    DrawButton(grc);

    DrawFocus(grc);

    return;
}

// ボタン描画
Void SNGUIButton::DrawButton(SNGraphicsContext* grc)
{
    SNGUI::ButtonBlockStatus button_sts = JudgeStatus();

    ButtonTiling(grc, SNGraphicsResButton, &SNGUI::ButtonBlockOffset[button_sts]);

    return;
}

// フォーカス描画
Void SNGUIButton::DrawFocus(SNGraphicsContext* grc)
{
    SNGUI::ButtonFocusStatus button_sts = JudgeFocusStatus();

    ButtonTiling(grc, SNGraphicsResFocus, &SNGUI::FocusBlockOffset[button_sts]);

    return;
}

Void SNGUIButton::ButtonTiling(SNGraphicsContext* grc, SNGraphicsResID res_id, const SNPoint* offset)
{
    SNBitmap* btn_img;
    SNRect dst_rect;
    SNRect src_rect;
    SNRect tiling_rect;
    SNGUI::ButtonBlockIndex index;
    Int32 block = SNSystemConfig::GUIBlockSize;

    btn_img = SNGraphicsResManager::GetResource(res_id);

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
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右上
    index = SNGUI::ButtonBlockRightUp;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 左下
    index = SNGUI::ButtonBlockLeftBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 右下
    index = SNGUI::ButtonBlockRightBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + dst_rect.Width - block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    ////////////////////////////////////////////
    // 上下共通
    tiling_rect.Width = dst_rect.Width - block * 2;
    tiling_rect.Height = block;

    // 上
    index = SNGUI::ButtonBlockUp;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    // 下
    index = SNGUI::ButtonBlockBottom;
    src_rect.PointX = SNGUI::ButtonBlockDef[index].X + offset->X;
    src_rect.PointY = SNGUI::ButtonBlockDef[index].Y + offset->Y;
    tiling_rect.PointX = dst_rect.PointX + block;
    tiling_rect.PointY = dst_rect.PointY + dst_rect.Height - block;
    SNGUI::Tiling(grc, &tiling_rect, btn_img, &src_rect);

    return;
}


SNGUI::ButtonBlockStatus SNGUIButton::JudgeStatus()
{
    SNGUI::ButtonBlockStatus ret = SNGUI::ButtonStatusNormal;

    if (NoFrame)
    {
        ret = SNGUI::ButtonStatusNoFrame;
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


SNGUI::ButtonFocusStatus SNGUIButton::JudgeFocusStatus()
{
    SNGUI::ButtonFocusStatus ret = SNGUI::FocusStatusNoFocus;

    if (!Focus)
    {
        ret = SNGUI::FocusStatusNoFocus;
    }
    else if (Push)
    {
        ret = SNGUI::FocusnStatusPush;
    }
    else
    {
        ret = SNGUI::FocusStatusNormal;
    }

    return ret;
}
