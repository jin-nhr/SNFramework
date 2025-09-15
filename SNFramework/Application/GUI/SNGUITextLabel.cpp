#include "SNGUITextLabel.h"
#include "../../System/SNWindowsAPI.h"
#include "../../Configuration/SNConfiguration.h"

// 状態クラス

// コンストラクタ
SNGUITextLabel::SNGUITextLabel()
{
	// 変数初期化
	TextColor = 0;
	ShadowEnable = false;
	ShadowColor = 0;
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
	Text.PreAllocate(SNConfiguration::SystemConfiguration.GUITextLabelLength);

	// 各種初期設定
	TextColor = RGB(255, 255, 255);
	ShadowEnable = true;
	ShadowColor = RGB(0, 0, 0);
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
		// テキストカラー設定
		::SetTextColor(
			(HDC)surface->GetDC(),
			ShadowColor);

		// テキスト描画
		TextOut(
			(HDC)surface->GetDC(),
			rect.PointX + ShadowOffset,
			rect.PointY + ShadowOffset,
			(LPCWSTR)Text.GetString(),
			Text.GetLength());
	}

	// テキストカラー設定
	::SetTextColor(
		(HDC)surface->GetDC(),
		TextColor);

	// テキスト描画
	TextOut(
		(HDC)surface->GetDC(),
		rect.PointX,
		rect.PointY,
		(LPCWSTR)Text.GetString(),
		Text.GetLength());

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
Void SNGUITextLabel::SetTextColor(UInt8 r, UInt8 g, UInt8 b)
{
	TextColor = RGB(r, g, b);

	return;
}

// 影有効設定
Void SNGUITextLabel::SetShadowEnable(Boolean shadow)
{
	ShadowEnable = shadow;

	return;
}

// 影色設定
Void SNGUITextLabel::SetShadowColor(UInt8 r, UInt8 g, UInt8 b)
{
	ShadowColor = RGB(r, g, b);

	return;
}

// 影オフセット設定
Void SNGUITextLabel::SetShadowOffset(Int32 offset)
{
	ShadowOffset = offset;

	return;
}
