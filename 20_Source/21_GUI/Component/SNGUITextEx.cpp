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
	WorkSurface = SNBitmapFont::GetWorkSurface();

	return;
}

Void SNGUITextEx::OnExit()
{
	SNBitmapFont::ReleaseWorkSurface(WorkSurface);

	return;
}

// 描画前処理
Void SNGUITextEx::OnPreDraw()
{
	// 規定クラスのPreDrawでテキストを更新しておく
	SNGUISystemTextEx::OnPreDraw();

	if (WorkSurface != nullptr)
	{
		// ワークサーフェスにフォント描画
		SNBitmapFont::PreDrawText(WorkSurface, Text.GetString(), Text.GetLength());
	}

	return;
}

// 描画処理
Void SNGUITextEx::OnDraw(SNGraphicsContext* grc)
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
