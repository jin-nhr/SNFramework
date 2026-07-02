#include "SNSystemPen.h"
#include "SNSystemColorTable.h"


// システムペン配列
SNPen SNSystemPen::Pen[SNColorNum];



// 初期化
Void SNSystemPen::Initialize()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		// ペン生成
		Pen[cnt].CreatePen((SNColor*)&SNSystemColorTable::ColorTable[cnt]);
	}

	return;
}

// 終了
Void SNSystemPen::Terminate()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Pen[cnt].DeletePen();
	}
	return;
}
