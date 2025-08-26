#include "SNGUIMonoRect.h"
#include "../../System/SNWindowsAPI.h"

// 白黒矩形

// コンストラクタ
SNGUIMonoRect::SNGUIMonoRect()
{
	// 変数初期化
	Whiteness = false;

	return;
}

// デストラクタ
SNGUIMonoRect::~SNGUIMonoRect()
{
	return;
}

// 初期化
Void SNGUIMonoRect::Initialize()
{
	Whiteness = false;

	return;
}

// 終了処理
Void SNGUIMonoRect::Terminate()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNGUIMonoRect::Step(SNEvent* event)
{
	return SNTransitionCodeNo;
}

// 描画処理
Void SNGUIMonoRect::Draw(SNSurface* surface)
{
	DWORD rop;
	SNRect rect = GetGlobalRect();

	// 色設定に応じて塗りつぶし色設定
	if (Whiteness)
	{
		rop = WHITENESS;
	}
	else
	{
		rop = BLACKNESS;
	}
	// BitBltで塗りつぶし
	::BitBlt(
		(HDC)surface->GetDC()->GetDeviceContext(),
		rect.PointX,
		rect.PointY,
		rect.Width,
		rect.Height,
		(HDC)surface->GetDC()->GetDeviceContext(),
		0,
		0,
		rop);

	return;
}

// 色設定 true:白 false:黒
Void SNGUIMonoRect::SetColor(Boolean whiteness)
{
	Whiteness = whiteness;

	return;
}
