#include "SNSystemBrush.h"
#include "SNSystemColorTable.h"


// システムブラシ配列
SNBrush SNSystemBrush::Brush[SNColorNum];



// 初期化
Void SNSystemBrush::Initialize()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		// ブラシ生成
		Brush[cnt].CreateBrush((SNColor*)&SNSystemColorTable::ColorTable[cnt]);
	}

	return;
}

// 終了
Void SNSystemBrush::Terminate()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Brush[cnt].DeleteBrush();
	}
	return;
}
