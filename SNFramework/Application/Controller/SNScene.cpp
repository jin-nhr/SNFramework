#include "SNScene.h"
#include "../../Configuration/SNConfiguration.h"

// シーンクラス

// コンストラクタ
SNScene::SNScene()
{
	Visible = true;
	ParentPosition = {0};
	ParentSize = {
		(Int32)SNConfiguration::SystemConfiguration.ScreenWidth,
		(Int32)SNConfiguration::SystemConfiguration.ScreenHeight };
	LocalPosition = {0};
	Size = {
		(Int32)SNConfiguration::SystemConfiguration.ScreenWidth,
		(Int32)SNConfiguration::SystemConfiguration.ScreenHeight };
	return;
}

// デストラクタ
SNScene::~SNScene()
{
	return;
}

// 初期化
Void SNScene::Initialize()
{
	OnInitialize();
	return;
}

// 終了処理
Void SNScene::Terminate()
{
	OnTerminate();
	return;
}

// Entry
Void SNScene::Entry()
{
	OnEntry();
	return;
}

// Exit
Void SNScene::Exit()
{
	OnExit();
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNScene::Step(SNEvent* event)
{
	return OnStep(event);
}

// 描画処理
Void SNScene::Draw(SNSurface* surface)
{
	// 表示状態時のみDraw実行
	if (Visible)
	{
		OnDraw(surface);
	}
	
	return;
}

// 座標/サイズ設定
Void SNScene::SetRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	LocalPosition.X = x;
	LocalPosition.Y = y;
	Size.Width = w;
	Size.Height = h;

	return;
}

// オフセット座標/サイズ取得
SNRect SNScene::GetRect()
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
Void SNScene::SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	ParentPosition.X = x;
	ParentPosition.Y = y;
	ParentSize.Width = w;
	ParentSize.Height = h;

	return;
}

// 親座標/サイズ設定
SNRect SNScene::GetParentRect()
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
Void SNScene::Move(Int32 x, Int32 y)
{
	// 移動量を加算
	LocalPosition.X += x;
	LocalPosition.Y += y;

	return;
}

// グローバル座標/サイズ取得
SNRect SNScene::CalcGlobalRect()
{
	SNRect rect;

	// グローバル座標に変換
	rect.PointX = ParentPosition.X + LocalPosition.X;
	rect.PointY = ParentPosition.Y + LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}


// 初期化
Void SNScene::OnInitialize()
{
	return;
}

// 終了処理
Void SNScene::OnTerminate()
{
	return;
}

// Entry
Void SNScene::OnEntry()
{
	return;
}

// Exit
Void SNScene::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNScene::OnStep(SNEvent* event)
{
	return SNTransitionCodeStay;
}

// 描画処理
Void SNScene::OnDraw(SNSurface* surface)
{
	return;
}
