#include "SNSystemSurface.h"
#include "SNSystemColorTable.h"
#include "SNSystemConfig.h"
#include "SNSystemBrush.h"

// システムペン配列
SNSurfaceDDB SNSystemSurface::Surface[SNColorNum];



// 初期化
Void SNSystemSurface::Initialize()
{
	Int32 cnt;
	Int32 width = SNSystemConfig::ScreenWidth;
	Int32 height = SNSystemConfig::ScreenHeight;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Surface[cnt].CreateSurface(width, height);

		// サーフェスをブラシで塗りつぶし
		SNSystemBrush::Brush[cnt].Fill(&Surface[cnt], 0, 0, width, height);
	}

	return;
}

// 終了
Void SNSystemSurface::Terminate()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Surface[cnt].DeleteSurface();
	}
	return;
}
