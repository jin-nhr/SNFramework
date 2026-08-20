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
	WorkSurface = nullptr;

	return;
}

// デストラクタ
SNGUIText::~SNGUIText()
{
	return;
}

Void SNGUIText::OnEntry()
{
	WorkSurface = SNBitmapFont::GetWorkSurface();

	return;
}

Void SNGUIText::OnExit()
{
	SNBitmapFont::ReleaseWorkSurface(WorkSurface);

	return;
}

// 描画前処理
Void SNGUIText::OnPreDraw()
{
	if (WorkSurface != nullptr)
	{
		// ワークサーフェスにフォント描画
		SNBitmapFont::PreDrawText(WorkSurface, Text.GetString(), Text.GetLength());
	}

	return;
}

// 描画処理
Void SNGUIText::OnDraw(SNGraphicsContext* grc)
{
	SNRect rect = CalcGlobalRect();

	if (WorkSurface != nullptr)
	{
		// カラーマトリクス変換
		SNBitmapFont::DrawExtraText(grc, WorkSurface, rect.PointX, rect.PointY, &Color, Text.GetLength());
	}

	else
	{
		// 規定クラスのOnDraw実行
		SNGUISystemText::OnDraw(grc);
	}

	return;
}
