#include "SNGUITextEx.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNGUI.h"
#include "SNGUIDef.h"

// コンストラクタ
SNGUITextEx::SNGUITextEx()
{
	// 変数初期化
	Value = 0;
	Update = true;

	Color = SNGUIDef::FontColor[SNGUIDef::FontColorNormal];

	return;
}

// デストラクタ
SNGUITextEx::~SNGUITextEx()
{
	return;
}



// テキスト設定
Void SNGUITextEx::SetText(String text)
{
	// 文字列設定
	Format.SetString(text);

	// リサイズのために呼んでおく(Valueで横方向はズレるが)
	SNGUIText::SetText(text);

	// 更新フラグセット
	Update = true;

	return;
}

// 値設定
Void SNGUITextEx::SetValue(Int64 value)
{
	// 値の変更あり
	if (value != Value)
	{
		// 値をセット
		Value = value;

		// 更新フラグセット
		Update = true;
	}

	return;
}


// 初期化
Void SNGUITextEx::OnInitialize()
{
	// ベースの初期化
	SNGUIText::OnInitialize();

	// 事前にメモリ確保
	Format.PreAllocate(SNSystemConfig::GUITextLabelLength);
	Value = 0;
	Update = true;

	return;
}

// 終了処理
Void SNGUITextEx::OnTerminate()
{
	// ベースの終了
	SNGUIText::OnTerminate();

	return;
}

Void SNGUITextEx::OnPreDraw()
{
	SNSize size;

	// 更新あり
	if (Update)
	{
		// 出力文字列をセット
		Text.Print((String)Format.GetString(), Value);

		// 文字列サイズ取得
		size = Text.GetStringImageSize();

		// 自身のサイズを変更
		Resize(size.Width, size.Height);

		// 更新フラグクリア
		Update = false;
	}

	return;
}


// 描画処理
Void SNGUITextEx::OnDraw()
{
	SNRect rect = CalcGlobalRect();

	// カラーマトリクス変換
	SNBitmapFont::DrawSystemText(rect.PointX, rect.PointY, Text.GetString(), Text.GetLength(), &Color);

	return;
}
