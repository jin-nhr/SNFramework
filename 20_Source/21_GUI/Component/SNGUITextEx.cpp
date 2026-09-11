#include "SNGUITextEx.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNGUI.h"
#include "SNGUIDef.h"

// コンストラクタ
SNGUITextEx::SNGUITextEx()
{
	// 変数初期化
	Color = SNGUIDef::FontColor[SNGUIDef::FontColorNormal];
	WorkSurface = nullptr;

	return;
}

// デストラクタ
SNGUITextEx::~SNGUITextEx()
{
	return;
}

Void SNGUITextEx::OnEntry()
{
	return;
}

Void SNGUITextEx::OnExit()
{
	return;
}

// 描画処理
Void SNGUITextEx::OnDraw()
{
	SNRect rect = CalcGlobalRect();

	// カラーマトリクス変換
	SNBitmapFont::DrawExtraText(rect.PointX, rect.PointY, &Color, Text.GetString(), Text.GetLength());

	return;
}
