#include "SNGUITextLabelEx.h"
#include "../../System/SNWindowsAPI.h"
#include "../../Configuration/SNConfiguration.h"

// 状態クラス

// コンストラクタ
SNGUITextLabelEx::SNGUITextLabelEx()
{
	// 変数初期化
	Value = 0;
	Update = true;

	return;
}

// デストラクタ
SNGUITextLabelEx::~SNGUITextLabelEx()
{
	return;
}

// テキスト設定
Void SNGUITextLabelEx::SetText(String text)
{
	// 文字列設定
	Format.SetString(text);

	// 更新フラグセット
	Update = true;

	return;
}

// 値設定
Void SNGUITextLabelEx::SetValue(Int64 value)
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
Void SNGUITextLabelEx::OnInitialize()
{
	// ベースの初期化
	SNGUITextLabel::OnInitialize();

	// 事前にメモリ確保
	Format.PreAllocate(SNConfiguration::SystemConfiguration.GUITextLabelLength);
	Value = 0;
	Update = true;

	return;
}

// 終了処理
Void SNGUITextLabelEx::OnTerminate()
{
	// ベースの終了
	SNGUITextLabel::OnTerminate();

	return;
}

// 描画処理
Void SNGUITextLabelEx::OnDraw(SNSurface* surface)
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
	SNGUITextLabel::OnDraw(surface);

	return;
}
