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
	Text.Free();
	return;
}

// 初期化
Void SNGUITextLabel::Initialize()
{
	UInt32 text_length = SNConfiguration::GetInstance()->ConfigurationData.System.GUITextLabelLength;

	Text.Allocate(sizeof(Char) * (text_length + 1));
	Text.Clear();
	TextColor = RGB(255, 255, 255);
	ShadowEnable = true;
	ShadowColor = RGB(0, 0, 0);
	ShadowOffset = 2;

	return;
}

// 終了処理
Void SNGUITextLabel::Terminate()
{
	Text.Free();
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNGUITextLabel::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNGUITextLabel::Draw(SNSurface* surface)
{
	SNRect rect;

	rect = GetGlobalRect();

	// 影描画あり
	if (ShadowEnable)
	{
		// テキストカラー設定
		::SetTextColor(
			(HDC)surface->GetDC()->GetDeviceContext(),
			ShadowColor);

		// テキスト描画
		TextOut(
			(HDC)surface->GetDC()->GetDeviceContext(),
			rect.PointX + ShadowOffset,
			rect.PointY + ShadowOffset,
			(LPCWSTR)Text.GetAddress(),
			lstrlen((LPCWSTR)Text.GetAddress()));
	}

	// テキストカラー設定
	::SetTextColor(
		(HDC)surface->GetDC()->GetDeviceContext(),
		TextColor);

	// テキスト描画
	TextOut(
		(HDC)surface->GetDC()->GetDeviceContext(),
		rect.PointX,
		rect.PointY,
		(LPCWSTR)Text.GetAddress(),
		lstrlen((LPCWSTR)Text.GetAddress()));

	return;
}


// テキスト設定
Void SNGUITextLabel::SetText(String text)
{
	UInt32 text_length = SNConfiguration::GetInstance()->ConfigurationData.System.GUITextLabelLength;

	// 文字列コピー
	wcscpy_s((String)Text.GetAddress(), text_length + 1, text);

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
