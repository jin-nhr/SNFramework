#include "SNState.h"


// 状態クラス

// コンストラクタ
SNState::SNState()
{
	return;
}

// デストラクタ
SNState::~SNState()
{
	return;
}

// 初期化
Void SNState::Initialize()
{
	return;
}

// 終了処理
Void SNState::Terminate()
{
	return;
}

// Entry
Void SNState::Entry()
{
	return;
}

// Exit
Void SNState::Exit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNState::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNState::Draw(SNSurface* surface)
{
	return;
}
