#include "SNGUIBase.h"


// 状態クラス

// コンストラクタ
SNGUIBase::SNGUIBase()
{
	// 変数初期化
	GlobalParentPosition = {0};
	ParentSize = {0};
	LocalPosition = {0};
	Size = {0};

	return;
}

// デストラクタ
SNGUIBase::~SNGUIBase()
{
	return;
}

// 初期化
Void SNGUIBase::Initialize()
{
	return;
}

// 終了処理
Void SNGUIBase::Terminate()
{
	return;
}

// Entry
Void SNGUIBase::Entry()
{
	return;
}

// Exit
Void SNGUIBase::Exit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNGUIBase::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNGUIBase::Draw(SNSurface* surface)
{
	return;
}

// 座標設定
Void SNGUIBase::SetPosition(Int32 x, Int32 y)
{
	LocalPosition.X = x;
	LocalPosition.Y = y;

	return;
}

// サイズ設定
Void SNGUIBase::SetSize(Int32 w, Int32 h)
{
	Size.Width = w;
	Size.Height = h;

	return;
}

// 座標、サイズ設定
Void SNGUIBase::SetRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	SetPosition(x, y);
	SetSize(w, h);

	return;
}

// 親座標設定
Void SNGUIBase::SetParentPosition(Int32 x, Int32 y)
{
	GlobalParentPosition.X = x;
	GlobalParentPosition.Y = y;

	return;
}

// 親サイズ設定
Void SNGUIBase::SetParentSize(Int32 width, Int32 height)
{
	ParentSize.Width = width;
	ParentSize.Height = height;

	return;
}

// 親座標、サイズ設定
Void SNGUIBase::SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	SetParentPosition(x, y);
	SetParentSize(w, h);

	return;
}

// 親、自身の座標、サイズ設定
// パラメータ：親、自身の座標、サイズを指定する
Void SNGUIBase::SetAllRect(Int32 px, Int32 py, Int32 pw, Int32 ph, Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 親と自身の座標、サイズを設定
	SetParentRect(px, py, pw, ph);
	SetRect(x, y, w, h);

	return;
}

// 移動
// パラメータ：移動量を指定する
Void SNGUIBase::Move(Int32 x, Int32 y)
{
	SNRect rect = GetOffsetRect();

	// 差分を加算した座標を設定
	SetPosition(rect.PointX + x, rect.PointY + y);

	return;
}

// センタリング
// パラメータ：横方向 or 縦方向への移動指定
Void SNGUIBase::Centering(Boolean horizontal, Boolean vertical)
{
	// 横方向
	if (horizontal)
	{
		// 親座標系の中央に配置する
		LocalPosition.X = (ParentSize.Width - Size.Width) /2;
	}

	// 縦方向
	if (vertical)
	{
		// 親座標系の中央に配置する
		LocalPosition.Y = (ParentSize.Height - Size.Height) / 2;
	}

	return;
}

// グローバル座標/サイズ取得
SNRect SNGUIBase::GetGlobalRect()
{
	SNRect rect;

	// グローバル座標に変換
	rect.PointX = GlobalParentPosition.X + LocalPosition.X;
	rect.PointY = GlobalParentPosition.Y + LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}

// オフセット座標/サイズ取得
SNRect SNGUIBase::GetOffsetRect()
{
	SNRect rect;

	// ローカル座標、サイズを返す
	rect.PointX = LocalPosition.X;
	rect.PointY = LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}
