#include "SNGUIText.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNGUI.h"
#include "SNGUIDef.h"

// コンストラクタ
SNGUIText::SNGUIText()
{
	// 変数初期化
	Color = SNGUIDef::FontColor[SNGUIDef::FontColorNormal];


	return;
}

// デストラクタ
SNGUIText::~SNGUIText()
{
	return;
}

Void SNGUIText::OnEntry()
{
	return;
}

Void SNGUIText::OnExit()
{
	return;
}

// 描画前処理
Void SNGUIText::OnPreDraw()
{
	return;
}

// 描画処理
Void SNGUIText::OnDraw()
{
	SNRect rect = CalcGlobalRect();

	// カラーマトリクス変換
	SNBitmapFont::DrawExtraText(rect.PointX, rect.PointY, &Color, Text.GetString(), Text.GetLength());


	return;
}
