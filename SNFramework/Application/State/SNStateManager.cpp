#include "SNStateManager.h"


// 状態管理クラス

// コンストラクタ
SNStateManager::SNStateManager()
{
	// 変数初期化
	NowState = 0;
	StateNum = 0;
	StateList = nullptr;
	TransitionInfo = nullptr;

	return;
}

// デストラクタ
SNStateManager::~SNStateManager()
{
	return;
}

// 初期化
Void SNStateManager::Initialize()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < StateNum; loop_cnt++)
	{
		StateList[loop_cnt]->Initialize();
	}

	return;
}

// 終了処理
Void SNStateManager::Terminate()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < StateNum; loop_cnt++)
	{
		StateList[loop_cnt]->Terminate();
	}

	return;
}

// Entry
Void SNStateManager::Entry()
{
	// 現在状態をEntryする
	StateList[NowState]->Entry();

	return;
}

// Exit
Void SNStateManager::Exit()
{
	// 現在状態をExitする
	StateList[NowState]->Exit();

	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNStateManager::Step(SNEvent* event)
{
	SNTransitionCode transition_code;
	UInt8 next_state;

	// 現在状態のStepを実行
	transition_code = StateList[NowState]->Step(event);

	// 遷移コードが有効範囲かチェック
	if (SNTransitionCode1 <= transition_code &&
		transition_code <= SNTransitionCode4)
	{
		// 遷移コードより遷移先状態を取得
		next_state = TransitionInfo[NowState].DestinationState[transition_code];

		// 状態数より大きい値の場合は遷移なしにする
		if (StateNum <= next_state)
		{
			next_state = NowState;
		}
	}
	else
	{
		// 範囲外のときは遷移なしにする
		next_state = NowState;
	}

	// 現在状態と異なる状態への遷移の場合
	if (next_state != NowState)
	{
		// 現在状態Exit
		StateList[NowState]->Exit();

		// 現在状態を更新
		NowState = next_state;

		// 遷移先状態Entry
		StateList[NowState]->Entry();
	}

	// リターンコードを還す
	return TransitionInfo[NowState].ReturnCode;
}

// 描画処理
Void SNStateManager::Draw(SNSurface* surface)
{
	// 現在状態の描画処理を実行
	StateList[NowState]->Draw(surface);

	return;
}

// 遷移情報設定 (遷移先情報を設定する)
Void SNStateManager::SetTransitionInfo(UInt8 state_num, SNState** state_list, SNStateTransitionInfo* transition_info)
{
	// 遷移情報を設定
	StateNum = state_num;
	StateList = state_list;
	TransitionInfo = transition_info;

	return;
}
