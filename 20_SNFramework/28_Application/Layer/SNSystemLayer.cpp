#include "SNSystemLayer.h"
#include "SNConfig.h"

// システムレイヤクラス

// コンストラクタ
SNSystemLayer::SNSystemLayer()
{
	return;
}

// デストラクタ
SNSystemLayer::~SNSystemLayer()
{
	return;
}

// 初期化
Void SNSystemLayer::OnInitialize()
{
	return;
}

// 終了処理
Void SNSystemLayer::OnTerminate()
{
	return;
}

// Entry
Void SNSystemLayer::OnEntry()
{
	return;
}

// Exit
Void SNSystemLayer::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNSystemLayer::OnStep(SNEvent* event)
{
	// 終了通知
	if (event->ApplicationEvent.ExitApplication)
	{
		event->Result.ExitApplication = true;
	}

	// 通常表示通知
	if (event->ApplicationEvent.NormalDisp)
	{
		// ユーザー設定を更新
		SNUserConfig::Data.FullScreen = false;
	}

	// 最大化表示通知
	if (event->ApplicationEvent.MaximizeDisp)
	{
		// ユーザー設定を更新
		SNUserConfig::Data.FullScreen = true;
	}

	return SNTransitionCodeStay;
}

// 描画処理
Void SNSystemLayer::OnDraw(SNSurface* surface)
{
	return;
}
