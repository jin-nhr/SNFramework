#include "SNGUILine.h"
#include "SNWindowsAPI.h"
#include "SNGraphics.h"
#include "SNSystemPen.h"
#include "SNMath.h"

// GUIラインクラス

// コンストラクタ
SNGUILine::SNGUILine()
{
	// 変数初期化
	Color = SNColorWhite;
	Point[0] = {0, 0};
	PointNum = 0;

	return;
}

// デストラクタ
SNGUILine::~SNGUILine()
{
	return;
}

// 初期化
Void SNGUILine::OnInitialize()
{
	Color = SNColorWhite;
	Point[0] = { 0, 0 };
	PointNum = 0;

	return;
}

// 終了処理
Void SNGUILine::OnTerminate()
{
	return;
}

// 色設定
Void SNGUILine::SetColor(SNColorCode color)
{
	Color = color;

	return;
}

// 座標設定
Void SNGUILine::SetPoint(SNPoint* point, UInt8 point_num)
{
	Int32 cnt;
	Int32 loop_max = (Int32)SNMath::SelectMin(point_num, PointMax);

	for (cnt = 0; cnt < loop_max; cnt++)
	{
		Point[cnt] = point[cnt];
	}

	PointNum = (UInt8)cnt;

	return;
}

// 描画処理
Void SNGUILine::OnDraw(SNSurface* surface)
{
	SNRect rect = CalcGlobalRect();

	// ライン描画
	SNSystemPen::Pen[Color].DrawLine(surface, Point, PointNum);

	return;
}
