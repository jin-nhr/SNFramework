#include "SNSystemColorTable.h"


const SNColor SNSystemColorTable::ColorTable[SNColorNum] =
{
	//	B,	G,	R,	A
		0,	0,	0,	0,	// Black
		128,128,128,0,	// Gray
		255,255,255,0,	// White
		0,	0,	128,0,	// DarkRed
		0,	128,0,	0,	// DarkGreen
		128,0,	0,	0,	// DarkBlue
		0,	128,128,0,	// DarkYellow
		128,0,	128,0,	// DarkPink
		128,128,0,	0,	// DarkAqua
		0,	0,	255,0,	// Red
		0,	255,0,	0,	// Green
		255,0,	0,	0,	// Blue
		0,	255,255,0,	// Yellow
		255,0,	255,0,	// Pink
		255,255,0,	0,	// Aqua
};


Void SNSystemColorTable::Initialize()
{
	return;
}

Void SNSystemColorTable::Terminate()
{
	return;
}

