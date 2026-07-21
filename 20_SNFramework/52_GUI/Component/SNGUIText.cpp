#include "SNGUIText.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"

// 状態クラス

// コンストラクタ
SNGUIText::SNGUIText()
{
	// 変数初期化

	return;
}

// デストラクタ
SNGUIText::~SNGUIText()
{
	return;
}

// 初期化
Void SNGUIText::OnInitialize()
{
	// 固定長でメモリ確保しておく
	Text.PreAllocate(SNSystemConfig::GUITextLabelLength);

	// 各種初期設定

	return;
}

// 終了処理
Void SNGUIText::OnTerminate()
{
	Text.Clear();
	return;
}

Void SNGUIText::OnEntry()
{

}

Void SNGUIText::OnExit()
{

}

// 描画前処理
Void SNGUIText::OnPreDraw()
{

}

// 描画処理
Void SNGUIText::OnDraw(SNGraphicsContext* grc)
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

Void SNGUIText::OnPostDraw()
{

}


// テキスト設定
Void SNGUIText::SetText(String text)
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
Void SNGUIText::SetText(SNBMString* text)
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
