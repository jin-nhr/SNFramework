#include "SNBackGroundLayer.h"
#include "../../Configuration/SNConfiguration.h"

// バックグラウンドレイヤクラス

// コンストラクタ
SNBackGroundLayer::SNBackGroundLayer()
{
	return;
}

// デストラクタ
SNBackGroundLayer::~SNBackGroundLayer()
{
	return;
}

// 初期化
Void SNBackGroundLayer::OnInitialize()
{
	return;
}

// 終了処理
Void SNBackGroundLayer::OnTerminate()
{
	return;
}

// Entry
Void SNBackGroundLayer::OnEntry()
{
	return;
}

// Exit
Void SNBackGroundLayer::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNBackGroundLayer::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;
}

// 描画処理
Void SNBackGroundLayer::OnDraw(SNSurface* surface)
{
	return;
}
