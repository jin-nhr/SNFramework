#include "SNApplicationNoUserApp.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"

// アプリケーション(ユーザー登録)

// コンストラクタ
SNApplicationNoUserApp::SNApplicationNoUserApp()
{
	// シーン登録
	SetSceneNum(2);
	SetScene(&Message);
	SetScene(&BackGround);

	return;
}

// デストラクタ
SNApplicationNoUserApp::~SNApplicationNoUserApp()
{
	return;
}

// 初期化
Void SNApplicationNoUserApp::OnInitialize()
{
	SNRect rect = CalcGlobalRect();

	// ラベル
	Message.SetRect(0, 0, Size.Width, Size.Height);
	Message.SetText((String)L"No user application registered.");

	Message.SetTextColor(SNColorWhite);
	Message.SetShadowColor(SNColorBlack);

	Message.Centering(true, true);

	// 背景
	BackGround.SetRect(0, 0, Size.Width, Size.Height);
	BackGround.SetColor(SNColorDarkBlue);

	return;
}

// 終了処理
Void SNApplicationNoUserApp::OnTerminate()
{
	return;
}

// Entry
Void SNApplicationNoUserApp::OnEntry()
{
	return;
}

// Exit
Void SNApplicationNoUserApp::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNApplicationNoUserApp::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;;
}

// 描画処理
Void SNApplicationNoUserApp::OnDraw(SNSurface* grc)
{
	return;
}
