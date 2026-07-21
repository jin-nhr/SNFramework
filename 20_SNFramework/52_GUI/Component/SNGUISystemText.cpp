#include "SNGUISystemText.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"

// 状態クラス

// コンストラクタ
SNGUISystemText::SNGUISystemText()
{
	// 変数初期化

	return;
}

// デストラクタ
SNGUISystemText::~SNGUISystemText()
{
	return;
}

// 初期化
Void SNGUISystemText::OnInitialize()
{
	// 固定長でメモリ確保しておく
	Text.PreAllocate(SNSystemConfig::GUITextLabelLength);

	// 各種初期設定

	return;
}

// 終了処理
Void SNGUISystemText::OnTerminate()
{
	Text.Clear();
	return;
}

// 描画処理
Void SNGUISystemText::OnDraw(SNGraphicsContext* grc)
{
	SNRect rect;

	rect = CalcGlobalRect();

	// 文字描画
	SNBitmapFont::DrawSystemText(
		grc,
		rect.PointX,
		rect.PointY,
		Text.GetString(),
		Text.GetLength());

	return;
}


// テキスト設定
Void SNGUISystemText::SetText(String text)
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
Void SNGUISystemText::SetText(SNBMString* text)
{
	SNSize size;

	// Stringに文字列設定
	Text.SetString(text->GetString(), text->GetLength());

	// 文字列サイズ取得
	size = Text.GetStringImageSize();

	// 自身のサイズを変更
	Resize(size.Width, size.Height);

	return;
}
