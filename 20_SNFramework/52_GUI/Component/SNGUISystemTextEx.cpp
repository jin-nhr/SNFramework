#include "SNGUISystemTextEx.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"

// 状態クラス

// コンストラクタ
SNGUISystemTextEx::SNGUISystemTextEx()
{
	// 変数初期化
	Value = 0;
	Update = true;

	return;
}

// デストラクタ
SNGUISystemTextEx::~SNGUISystemTextEx()
{
	return;
}

// テキスト設定
Void SNGUISystemTextEx::SetText(String text)
{
	// 文字列設定
	Format.SetString(text);

	// リサイズのために呼んでおく(Valueで横方向はズレるが)
	SNGUISystemText::SetText(text);

	// 更新フラグセット
	Update = true;

	return;
}

// 値設定
Void SNGUISystemTextEx::SetValue(Int64 value)
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
Void SNGUISystemTextEx::OnInitialize()
{
	// ベースの初期化
	SNGUISystemText::OnInitialize();

	// 事前にメモリ確保
	Format.PreAllocate(SNSystemConfig::GUITextLabelLength);
	Value = 0;
	Update = true;

	return;
}

// 終了処理
Void SNGUISystemTextEx::OnTerminate()
{
	// ベースの終了
	SNGUISystemText::OnTerminate();

	return;
}

Void SNGUISystemTextEx::OnPreDraw()
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
