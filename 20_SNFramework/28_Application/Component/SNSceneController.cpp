#include "SNSceneController.h"
#include "SNConfig.h"

// シーン制御クラス

// コンストラクタ
SNSceneController::SNSceneController()
{
	return;
}

// デストラクタ
SNSceneController::~SNSceneController()
{
	SceneList.Free();

	return;
}

// シーン数設定
Void SNSceneController::SetSceneNum(UInt32 scene_num)
{
	// メモリ確保
	SceneList.Allocate(scene_num);

	return;
}

// シーン登録
Void SNSceneController::SetScene(SNScene* scene)
{
	SNRect rect = CalcGlobalRect();

	// リストの終端へ追加しsceneを登録
	SceneList.InsertLast()->UserData = (Void*)scene;

	// 親座標設定
	scene->SetParentRect(rect.PointX, rect.PointY, rect.Width, rect.Height);

	return;
}


// 座標/サイズ設定
Void SNSceneController::SetRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	LocalPosition.X = x;
	LocalPosition.Y = y;
	Size.Width = w;
	Size.Height = h;

	// 子情報更新
	UpdateChildRect();

	return;
}

// オフセット座標/サイズ取得
SNRect SNSceneController::GetRect()
{
	SNRect rect;

	// ローカル座標、サイズを返す
	rect.PointX = LocalPosition.X;
	rect.PointY = LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}

// 親座標/サイズ設定
Void SNSceneController::SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	ParentPosition.X = x;
	ParentPosition.Y = y;
	ParentSize.Width = w;
	ParentSize.Height = h;

	// 子情報更新
	UpdateChildRect();

	return;
}

// 親座標/サイズ取得
SNRect SNSceneController::GetParentRect()
{
	SNRect rect;

	// ローカル座標、サイズを返す
	rect.PointX = ParentPosition.X;
	rect.PointY = ParentPosition.Y;
	rect.Width = ParentSize.Width;
	rect.Height = ParentSize.Height;

	return rect;
}

// 移動
// パラメータ：移動量を指定する
Void SNSceneController::Move(Int32 x, Int32 y)
{
	// 移動量を加算
	LocalPosition.X += x;
	LocalPosition.Y += y;

	// 子情報更新
	UpdateChildRect();

	return;
}

// リサイズ
Void SNSceneController::Resize(Int32 w, Int32 h)
{
	// サイズ設定
	Size.Width = w;
	Size.Height = h;

	// 子情報更新
	UpdateChildRect();

	return;
}

// センタリング処理
Void SNSceneController::Centering(Boolean v, Boolean h)
{
	// 垂直方向
	if (v)
	{
		LocalPosition.Y = (ParentSize.Height - Size.Height) / 2;
	}

	// 水平方向
	if (h)
	{
		LocalPosition.X = (ParentSize.Width - Size.Width) / 2;
	}

	// 子情報更新
	UpdateChildRect();

	return;
}

// グローバル座標/サイズ計算
SNRect SNSceneController::CalcGlobalRect()
{
	SNRect rect;

	// グローバル座標に変換
	rect.PointX = ParentPosition.X + LocalPosition.X;
	rect.PointY = ParentPosition.Y + LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}

// 子情報更新
Void SNSceneController::UpdateChildRect()
{
	SNRect rect;
	SNListContainer* it = SceneList.GetTop();

	// グローバル情報計算
	rect = CalcGlobalRect();

	while (it != nullptr)
	{
		((SNScene*)it->UserData)->SetParentRect(rect.PointX, rect.PointY, rect.Width, rect.Height);
		it = it->Next;
	}

	return;
}
