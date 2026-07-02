#include "SNGUITextLabel.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNGraphics.h"
#include "SNSystemColorTable.h"
#include "SNGDI.h"

// 状態クラス

// コンストラクタ
SNGUITextLabel::SNGUITextLabel()
{
	// 変数初期化
	TextColor = SNColorWhite;
	ShadowEnable = false;
	ShadowColor = SNColorBlack;
	ShadowOffset = 0;

	return;
}

// デストラクタ
SNGUITextLabel::~SNGUITextLabel()
{
	return;
}

// 初期化
Void SNGUITextLabel::OnInitialize()
{
	// 固定長でメモリ確保しておく
	Text.PreAllocate(SNSystemConfig::GUITextLabelLength);

	// 各種初期設定
	TextColor = SNColorWhite;
	ShadowEnable = true;
	ShadowColor = SNColorBlack;
	ShadowOffset = 2;

	return;
}

// 終了処理
Void SNGUITextLabel::OnTerminate()
{
	Text.Clear();
	return;
}

// 描画処理
Void SNGUITextLabel::OnDraw(SNSurface* surface)
{
	SNRect rect;

	rect = CalcGlobalRect();

	// 影描画あり
	if (ShadowEnable)
	{
		SNGDI gdi;

		gdi.DrawString(
			surface->GetDC(),
			rect.PointX + ShadowOffset,
			rect.PointY + ShadowOffset,
			Text.GetString(),
			Text.GetLength(),
			(SNColor*)&SNSystemColorTable::ColorTable[ShadowColor]);
	}

	// 文字列本体
	{
		SNGDI gdi;

		gdi.DrawString(
			surface->GetDC(),
			rect.PointX,
			rect.PointY,
			Text.GetString(),
			Text.GetLength(),
			(SNColor*)&SNSystemColorTable::ColorTable[TextColor]);
	}

	return;
}


// テキスト設定
Void SNGUITextLabel::SetText(String text)
{
	// Stringに文字列設定
	Text.SetString(text);

	return;
}

// テキスト色設定
Void SNGUITextLabel::SetTextColor(SNColorCode color)
{
	TextColor = color;

	return;
}

// 影有効設定
Void SNGUITextLabel::SetShadowEnable(Boolean shadow)
{
	ShadowEnable = shadow;

	return;
}

// 影色設定
Void SNGUITextLabel::SetShadowColor(SNColorCode color)
{
	ShadowColor = color;

	return;
}

// 影オフセット設定
Void SNGUITextLabel::SetShadowOffset(Int32 offset)
{
	ShadowOffset = offset;

	return;
}
