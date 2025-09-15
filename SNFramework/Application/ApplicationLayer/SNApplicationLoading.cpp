#include "SNApplicationLoading.h"
#include "../../System/SNWindowsAPI.h"
#include "../../Configuration/SNConfiguration.h"


// アプリケーション(ローディング)

// コンストラクタ
SNApplicationLoading::SNApplicationLoading()
{
	// シーン登録
	SetSceneNum(2);
	SetScene(&Message);
	SetScene(&BackGround);

	return;
}

// デストラクタ
SNApplicationLoading::~SNApplicationLoading()
{
	return;
}

// 初期化
Void SNApplicationLoading::OnInitialize()
{
	SNRect rect = CalcGlobalRect();

	// ラベル
	Message.SetRect(0, 0, Size.Width, Size.Height);
	Message.SetText((String)L"Loading...");

	// 背景
	BackGround.SetRect(0, 0, Size.Width, Size.Height);
	BackGround.SetColor(false);

	return;
}

// 終了処理
Void SNApplicationLoading::OnTerminate()
{
	return;
}

// Entry
Void SNApplicationLoading::OnEntry()
{
	return;
}

// Exit
Void SNApplicationLoading::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNApplicationLoading::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;
}

// 描画処理
Void SNApplicationLoading::OnDraw(SNSurface* surface)
{
	return;
}
