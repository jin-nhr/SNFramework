#include "SNLayerController.h"
#include "../../Configuration/SNConfiguration.h"

// レイヤ制御クラス

// コンストラクタ
SNLayerController::SNLayerController()
{
	return;
}

// デストラクタ
SNLayerController::~SNLayerController()
{
	return;
}

// 初期化
Void SNLayerController::Initialize()
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

	// 自身のInitialize実行
	OnInitialize();

	return;
}

// 終了処理
Void SNLayerController::Terminate()
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

	// 自身のTerminate実行
	OnTerminate();

	return;
}

// Entry
Void SNLayerController::Entry()
{
	SNListContainer* list_it;

	// 自身のEntry実行
	OnEntry();

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Entry();
		list_it = list_it->Next;
	}

	return;
}

// Exit
Void SNLayerController::Exit()
{
	SNListContainer* list_it;

	// 自身のExit実行
	OnExit();

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Exit();
		list_it = list_it->Next;
	}

	return;
}

// 1フレーム実行
// リターン：遷移先コード -1:遷移なし
SNTransitionCode SNLayerController::Step(SNEvent* event)
{
	SNListContainer* list_it;

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Step(event);
		list_it = list_it->Next;
	}

	// 自身のStep実行しリターンコードを還す
	return OnStep(event);
}

// 描画処理
Void SNLayerController::Draw(SNSurface* surface)
{
	SNListContainer* list_it;

	// 表示状態のときのみ処理
	if (Visible)
	{
		// 先頭取得
		list_it = SceneList.GetLast();

		// 先頭から終端まで処理
		while (list_it != nullptr)
		{
			((SNScene*)list_it->UserData)->Draw(surface);
			list_it = list_it->Prev;
		}

		// 自身の描画処理
		OnDraw(surface);
	}

	return;
}


// 初期化
Void SNLayerController::OnInitialize()
{
	return;
}

// 終了処理
Void SNLayerController::OnTerminate()
{
	return;
}

// Entry
Void SNLayerController::OnEntry()
{
	return;
}

// Exit
Void SNLayerController::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
SNTransitionCode SNLayerController::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;
}

// 描画処理(実装用)
Void SNLayerController::OnDraw(SNSurface* surface)
{
	return;
}
