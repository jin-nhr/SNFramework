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
    SNRect rect = CalcGlobalRect();

    SNGUI::Tiling9(grc, &rect, res_id, SNGUI::GUIBlockDef, offset);

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
