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
	Format.Free();
	return;
}

// 初期化
Void SNGUITextLabelEx::Initialize()
{
	UInt32 text_length = SNConfiguration::GetInstance()->ConfigurationData.System.GUITextLabelLength;

	SNGUITextLabel::Initialize();

	Format.Allocate(sizeof(Char) * (text_length + 1));
	Format.Clear();
	Value = 0;
	Update = true;

	return;
}

// 終了処理
Void SNGUITextLabelEx::Terminate()
{
	SNGUITextLabel::Terminate();

	Format.Free();

	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNGUITextLabelEx::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNGUITextLabelEx::Draw(SNSurface* surface)
{
	UInt32 text_length = SNConfiguration::GetInstance()->ConfigurationData.System.GUITextLabelLength;

	// 更新あり
	if (Update)
	{
		// 出力文字列をセット
		StringCchPrintfW((String)Text.GetAddress(), text_length + 1, (String)Format.GetAddress(), Value);

		// 更新フラグクリア
		Update = false;
	}

	// ベースのDraw実行
	SNGUITextLabel::Draw(surface);

	return;
}

// テキスト設定
Void SNGUITextLabelEx::SetText(String text)
{
	UInt32 text_length = SNConfiguration::GetInstance()->ConfigurationData.System.GUITextLabelLength;

	// 文字列コピー
	wcscpy_s((String)Format.GetAddress(), text_length + 1, text);

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
