#include "SNGUIBMTextLabel.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"

// 状態クラス

// コンストラクタ
SNGUIBMTextLabel::SNGUIBMTextLabel()
{
	// 変数初期化
	TextColor = SNColorWhite;
	ShadowEnable = false;
	ShadowColor = SNColorBlack;
	ShadowOffset = 0;

	return;
}

// デストラクタ
SNGUIBMTextLabel::~SNGUIBMTextLabel()
{
	return;
}

// 初期化
Void SNGUIBMTextLabel::OnInitialize()
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
Void SNGUIBMTextLabel::OnTerminate()
{
	Text.Clear();
	return;
}

// 描画処理
Void SNGUIBMTextLabel::OnDraw(SNSurface* surface)
{
	SNRect rect;

	rect = CalcGlobalRect();

	// 影描画あり
	if (ShadowEnable)
	{
		// 影描画
		SNBitmapFont::Draw(
			surface,
			rect.PointX + ShadowOffset,
			rect.PointY + ShadowOffset,
			Text.GetString(),
			Text.GetLength(),
			ShadowColor);
	}

	// 影描画
	SNBitmapFont::Draw(
		surface,
		rect.PointX,
		rect.PointY,
		Text.GetString(),
		Text.GetLength(),
		TextColor);

	return;
}


// テキスト設定
Void SNGUIBMTextLabel::SetText(String text)
{
	SNSize size;

	// Stringに文字列設定
	Text.SetString(text);

	// 文字列サイズ取得
	size = Text.GetStringImageSize();

	// 自身のサイズを変更
	Resize(size.Width, size.Height);

	return;
}

// テキスト設定
Void SNGUIBMTextLabel::SetText(SNBMString* text)
{
	// Stringに文字列設定
	Text.SetString(text->GetString(), text->GetLength());

	return;
}

// テキスト色設定
Void SNGUIBMTextLabel::SetTextColor(SNColorCode color)
{
	TextColor = color;

	return;
}

// 影有効設定
Void SNGUIBMTextLabel::SetShadowEnable(Boolean shadow)
{
	ShadowEnable = shadow;

	return;
}

// 影色設定
Void SNGUIBMTextLabel::SetShadowColor(SNColorCode color)
{
	ShadowColor = color;

	return;
}

// 影オフセット設定
Void SNGUIBMTextLabel::SetShadowOffset(Int32 offset)
{
	ShadowOffset = offset;

	return;
}
