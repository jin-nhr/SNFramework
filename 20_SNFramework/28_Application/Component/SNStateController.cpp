#include "SNStateController.h"

// 遷移先情報
struct SNTransitionInfo
{
	UInt8 DistStateNo[SNTransitionCodeNum];	// 遷移先状態番号
};

// 状態制御クラス

// コンストラクタ
SNStateController::SNStateController()
{
	// 変数初期化
	NowState = 0;

	return;
}

// デストラクタ
SNStateController::~SNStateController()
{
	return;
}

// シーン数設定(登録できる数を決める)
Void SNStateController::SetSceneNum(UInt32 scene_num)
{
	// シーン登録数を設定
	SNSceneController::SetSceneNum(scene_num);
	
	// 遷移先情報のリスト登録数を設定
	TransitionList.Allocate(scene_num);

	return;
}

// 初期化
Void SNStateController::Initialize()
{
	SNListContainer* list_it;

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Initialize();
		list_it = list_it->Next;
	}

	// 自身のSceneのInitialize実行
	SNScene::Initialize();

	return;
}

// 終了処理
Void SNStateController::Terminate()
{
	SNListContainer* list_it;

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Terminate();
		list_it = list_it->Next;
	}

	// 自身のSceneのTerminate実行
	SNScene::Terminate();

	return;
}

// Entry
Void SNStateController::Entry()
{
	// 自身のSceneのEntryを実行
	SNScene::Entry();

	// 現在状態をEntryする
	((SNScene*)SceneList.DirectAccess(NowState)->UserData)->Entry();

	return;
}

// Exit
Void SNStateController::Exit()
{
	// 現在状態をExitする
	((SNScene*)SceneList.DirectAccess(NowState)->UserData)->Exit();

	// 自身のSceneのExitを実行
	SNScene::Exit();

	return;
}

// 1フレーム実行
Void SNStateController::Step()
{
	SNTransitionCode transition_code;
	UInt8 next_state;
	SNScene* proc_scene;

	// 有効時のみ処理
	if (Enable)
	{
		// 現在状態のSceneを取得
		proc_scene = ((SNScene*)SceneList.DirectAccess(NowState)->UserData);

		// Stepを実行
		proc_scene->Step();

		// 遷移先コード取得
		transition_code = proc_scene->TransCode;

		// 遷移コードが有効範囲かチェック
		if (SNTransitionCodeTop <= transition_code &&
			transition_code <= SNTransitionCodeLast)
		{
			// 遷移コードより遷移先状態を取得
			next_state = (UInt8)((SNTransitionInfo*)(&TransitionList.DirectAccess(NowState)->UserData))->DistStateNo[transition_code];

			// 状態数より大きい値の場合は遷移なしにする
			if (SceneList.GetNum() <= next_state)
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
			((SNScene*)SceneList.DirectAccess(NowState)->UserData)->Exit();

			// 現在状態を更新
			NowState = next_state;

			// 遷移先状態Entry
			((SNScene*)SceneList.DirectAccess(NowState)->UserData)->Entry();
		}

		// 自身のSceneのStepを実行
		SNScene::Step();
	}
	return;
}

// 描画処理
Void SNStateController::Draw(SNGraphicsContext* grc)
{
	// 有効時
	if (Enable)
	{
		// 表示状態のときのみ処理
		if (Visible)
		{
			// 自身のSceneの描画処理
			SNScene::Draw(grc);

			// 現在状態の描画処理を実行
			((SNScene*)SceneList.DirectAccess(NowState)->UserData)->Draw(grc);
		}
	}
	return;
}

// シーン設定
// sceneにはサブ状態となるシーンを設定する
// dist1～4はTransitionCode1～4に該当する遷移先Noを入れる
// 遷移先No = SetSceneの登録順と一致する
// つまり最初に登録したSceneが0, 次が1, その次が2 ... となる
Void SNStateController::SetScene(SNScene* scene, UInt8 dist1, UInt8 dist2, UInt8 dist3, UInt8 dist4)
{
	SNTransitionInfo* trans_inf;

	// リスト追加してUserDataのアドレス取得
	trans_inf = (SNTransitionInfo*)&TransitionList.InsertLast()->UserData;

	// UserDataに遷移先情報をセット (4Byte分のデータなので収まる)
	trans_inf->DistStateNo[0] = dist1;
	trans_inf->DistStateNo[1] = dist2;
	trans_inf->DistStateNo[2] = dist3;
	trans_inf->DistStateNo[3] = dist4;

	// シーンを登録
	SNSceneController::SetScene(scene);

	return;
}

// シーン設定完了
// シーンの登録がすべて完了した後で実行すること
Void SNStateController::CompleteSceneSet()
{
	// リストアを行い直接アクセスに備える
	TransitionList.Restore();
	SceneList.Restore();

	return;
}

