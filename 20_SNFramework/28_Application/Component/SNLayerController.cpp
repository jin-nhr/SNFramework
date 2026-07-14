#include "SNLayerController.h"
#include "SNConfig.h"

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
	SNScene::Initialize();

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
	SNScene::Terminate();

	return;
}

// Entry
Void SNLayerController::Entry()
{
	SNListContainer* list_it;

	// 自身のEntry実行
	SNScene::Entry();

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

	// 先頭取得
	list_it = SceneList.GetTop();

	// 先頭から終端まで処理
	while (list_it != nullptr)
	{
		((SNScene*)list_it->UserData)->Exit();
		list_it = list_it->Next;
	}

	// 自身のExit実行
	SNScene::Exit();

	return;
}

// 1フレーム実行
Void SNLayerController::Step()
{
	SNListContainer* list_it;

	// 有効時
	if (Enable)
	{
		// 先頭取得
		list_it = SceneList.GetTop();

		// 先頭から終端まで処理
		while (list_it != nullptr)
		{
			((SNScene*)list_it->UserData)->Step();
			list_it = list_it->Next;
		}

		// 自身のSceneのStep実行
		SNScene::Step();
	}

	return;
}

// 描画処理
Void SNLayerController::Draw(SNGraphicsContext* grc)
{
	SNListContainer* list_it;

	if (Enable)
	{
		// 表示状態のときのみ処理
		if (Visible)
		{
			// 自身の描画処理
			SNScene::Draw(grc);

			// 最終取得
			list_it = SceneList.GetLast();

			// 最終から先頭まで処理
			while (list_it != nullptr)
			{
				((SNScene*)list_it->UserData)->Draw(grc);
				list_it = list_it->Prev;
			}
		}
	}

	return;
}

