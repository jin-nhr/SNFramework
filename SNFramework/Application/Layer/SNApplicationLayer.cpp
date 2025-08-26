#include "SNApplicationLayer.h"
#include "../../System/SNWindowsAPI.h"
#include "../../Configuration/SNConfiguration.h"


// アプリケーションレイヤクラス

// 未登録メッセージ
const String SNApplicationLayer::NoAppRegMessage = (String)L"No User App Registered";

// コンストラクタ
SNApplicationLayer::SNApplicationLayer()
{
	Int32 cnt;

	// 変数初期化
	for (cnt = 0; cnt < GUIPartsNum; cnt++)
	{
		GUIPartsList[cnt] = nullptr;
	}
	return;
}

// デストラクタ
SNApplicationLayer::~SNApplicationLayer()
{
	return;
}

// 初期化
Void SNApplicationLayer::Initialize()
{
	SNSystemConfiguration* sdt = &SNConfiguration::GetInstance()->ConfigurationData.System;

	// コンテナに設定
	// GUI部品リスト設定
	GUIPartsList[0] = &NoAppRegistText;
	GUIPartsList[1] = &BackGround;
	GUIContainer.SetGUIPartsInfo(GUIPartsNum, GUIPartsList);
	GUIContainer.Initialize();

	// コンテナの座標設定
	GUIContainer.SetPosition(0, 0);

	// ラベル
	NoAppRegistText.SetPosition(0, 0);
	NoAppRegistText.SetSize(sdt->ScreenWidth, sdt->ScreenHeight);
	NoAppRegistText.SetText(NoAppRegMessage);

	// 背景
	BackGround.SetPosition(0, 0);
	BackGround.SetSize(sdt->ScreenWidth, sdt->ScreenHeight);
	BackGround.SetColor(false);

	return;
}

// 終了処理
Void SNApplicationLayer::Terminate()
{
	GUIContainer.Terminate();

	return;
}

// Entry
Void SNApplicationLayer::Entry()
{
	GUIContainer.Entry();

	return;
}

// Exit
Void SNApplicationLayer::Exit()
{
	GUIContainer.Exit();

	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNApplicationLayer::Step(SNEvent* event)
{
	GUIContainer.Step(event);

	return SNTransitionCodeNo;;
}

// 描画処理
Void SNApplicationLayer::Draw(SNSurface* surface)
{
	GUIContainer.Draw(surface);

	return;
}
