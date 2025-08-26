#include "SNSystemLayer.h"
#include "../../Configuration/SNConfiguration.h"

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
Void SNSystemLayer::Initialize()
{
	return;
}

// 終了処理
Void SNSystemLayer::Terminate()
{
	return;
}

// Entry
Void SNSystemLayer::Entry()
{
	return;
}

// Exit
Void SNSystemLayer::Exit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNSystemLayer::Step(SNEvent* event)
{
	// 終了通知
	if (event->ApplicationEvent->ExitApplication)
	{
		event->Result.ExitApplication = true;
	}

	return SNTransitionCodeNo;
}

// 描画処理
Void SNSystemLayer::Draw(SNSurface* surface)
{
	return;
}
