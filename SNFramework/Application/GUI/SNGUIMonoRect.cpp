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
Void SNGUIMonoRect::OnInitialize()
{
	Whiteness = false;

	return;
}

// 終了処理
Void SNGUIMonoRect::OnTerminate()
{
	return;
}

// 描画処理
Void SNGUIMonoRect::OnDraw(SNSurface* surface)
{
	DWORD rop;
	SNRect rect = CalcGlobalRect();

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
		(HDC)surface->GetDC(),
		rect.PointX,
		rect.PointY,
		rect.Width,
		rect.Height,
		(HDC)surface->GetDC(),
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
