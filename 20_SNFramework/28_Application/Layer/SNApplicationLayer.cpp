#include "SNApplicationLayer.h"
#include "SNWindowsAPI.h"
#include "SNUserApplicationBase.h"

// アプリケーションレイヤクラス

// コンストラクタ
SNApplicationLayer::SNApplicationLayer()
{
	return;
}

// デストラクタ
SNApplicationLayer::~SNApplicationLayer()
{
	return;
}

// 初期化
Void SNApplicationLayer::OnInitialize()
{
	// シーン登録はコンストラクタでやっておきたいが
	// ユーザーアプリを確認するためにInitializeで実行する
	SNScene* user_app = SNUserApplicationBase::UserApplication;

	if (user_app != nullptr)
	{
		// Initializeで登録する場合は事前にInitializeしておく
		LoadingApp.Initialize();
		user_app->Initialize();

		// 登録
		SetSceneNum(2);
		SetScene(&LoadingApp, 1, 0, 0, 0);
		SetScene(user_app, 0, 0, 0, 0);
	}

	else
	{
		// Initializeで登録する場合は事前にInitializeしておく
		LoadingApp.Initialize();
		NoUserApp.Initialize();

		// 登録
		SetSceneNum(2);
		SetScene(&LoadingApp, 1, 0, 0, 0);
		SetScene(&NoUserApp, 0, 0, 0, 0);
	}

	return;
}

// 終了処理
Void SNApplicationLayer::OnTerminate()
{
	return;
}

// Entry
Void SNApplicationLayer::OnEntry()
{
	return;
}

// Exit
Void SNApplicationLayer::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNApplicationLayer::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;
}

// 描画処理
Void SNApplicationLayer::OnDraw(SNSurface* surface)
{
	return;
}
