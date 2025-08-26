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
Void SNBackGroundLayer::Initialize()
{
	return;
}

// 終了処理
Void SNBackGroundLayer::Terminate()
{
	return;
}

// Entry
Void SNBackGroundLayer::Entry()
{
	return;
}

// Exit
Void SNBackGroundLayer::Exit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNBackGroundLayer::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNBackGroundLayer::Draw(SNSurface* surface)
{
	return;
}
