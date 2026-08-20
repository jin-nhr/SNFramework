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
    SNGUIDef::ButtonBlockStatus button_sts = JudgeStatus();

    Caption.Color = SNGUIDef::FontColor[SNGUIDef::ButtonCaptionColor[button_sts]];

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
    SNGUIDef::ButtonBlockStatus button_sts = JudgeStatus();

    ButtonTiling(grc, SNGraphicsResButton, &SNGUIDef::ButtonBlockOffset[button_sts]);

    return;
}

// フォーカス描画
Void SNGUIButton::DrawFocus(SNGraphicsContext* grc)
{
    SNGUIDef::ButtonFocusStatus button_sts = JudgeFocusStatus();

    ButtonTiling(grc, SNGraphicsResFocus, &SNGUIDef::FocusBlockOffset[button_sts]);

    return;
}

Void SNGUIButton::ButtonTiling(SNGraphicsContext* grc, SNGraphicsResID res_id, const SNPoint* offset)
{
    SNRect rect = CalcGlobalRect();

    SNGUI::Tiling9(grc, &rect, res_id, SNGUIDef::GUIBlockDef, offset);

    return;
}


SNGUIDef::ButtonBlockStatus SNGUIButton::JudgeStatus()
{
    SNGUIDef::ButtonBlockStatus ret = SNGUIDef::ButtonStatusNormal;

    if (NoFrame)
    {
        ret = SNGUIDef::ButtonStatusNoFrame;
    }

    else if (Disable)
    {
        ret = SNGUIDef::ButtonStatusDisable;
    }

    else if (Push)
    {
        ret = SNGUIDef::ButtonStatusPush;
    }

    else if (Selected)
    {
        ret = SNGUIDef::ButtonStatusSelected;
    }

    return ret;
}


SNGUIDef::ButtonFocusStatus SNGUIButton::JudgeFocusStatus()
{
    SNGUIDef::ButtonFocusStatus ret = SNGUIDef::FocusStatusNoFocus;

    if (!Focus)
    {
        ret = SNGUIDef::FocusStatusNoFocus;
    }
    else if (Push)
    {
        ret = SNGUIDef::FocusnStatusPush;
    }
    else
    {
        ret = SNGUIDef::FocusStatusNormal;
    }

    return ret;
}
