#include "SNGUIBMTextLabelEx.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"

// 状態クラス

// コンストラクタ
SNGUIBMTextLabelEx::SNGUIBMTextLabelEx()
{
	// 変数初期化
	Value = 0;
	Update = true;

	return;
}

// デストラクタ
SNGUIBMTextLabelEx::~SNGUIBMTextLabelEx()
{
	return;
}

// テキスト設定
Void SNGUIBMTextLabelEx::SetText(String text)
{
	// 文字列設定
	Format.SetString(text);

	// 更新フラグセット
	Update = true;

	return;
}

// 値設定
Void SNGUIBMTextLabelEx::SetValue(Int64 value)
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
Void SNGUIBMTextLabelEx::OnInitialize()
{
	// ベースの初期化
	SNGUIBMTextLabel::OnInitialize();

	// 事前にメモリ確保
	Format.PreAllocate(SNSystemConfig::GUITextLabelLength);
	Value = 0;
	Update = true;

	return;
}

// 終了処理
Void SNGUIBMTextLabelEx::OnTerminate()
{
	// ベースの終了
	SNGUIBMTextLabel::OnTerminate();

	return;
}

// 描画処理
Void SNGUIBMTextLabelEx::OnDraw(SNSurface* surface)
{
	// 更新あり
	if (Update)
	{
		// 出力文字列をセット
		Text.Print((String)Format.GetString(), Value);

		// 更新フラグクリア
		Update = false;
	}

	// ベースのDraw実行
	SNGUIBMTextLabel::OnDraw(surface);

	return;
}
