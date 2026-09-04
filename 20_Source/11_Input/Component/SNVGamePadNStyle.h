#pragma once
#include "SNFrameworkInternal.h"
#include "SNVirtualDirectGamePad.h"

// 仮想ゲームパッドクラス(Nスタイル)
class SNVGamePadNStyle
{
public:
	SNVGamePadNStyle();
	virtual ~SNVGamePadNStyle();

	virtual Void SetID(SNVirtualGamePadID id);

	// Dir上
	virtual Boolean DPadUpPush();
	virtual Boolean DPadUpPress();
	virtual Boolean DPadUpRepeat();
	virtual Boolean DPadUpLong();
	virtual Boolean DPadUpRelease();

	// Dir下
	virtual Boolean DPadDownPush();
	virtual Boolean DPadDownPress();
	virtual Boolean DPadDownRepeat();
	virtual Boolean DPadDownLong();
	virtual Boolean DPadDownRelease();

	// Dir左
	virtual Boolean DPadLeftPush();
	virtual Boolean DPadLeftPress();
	virtual Boolean DPadLeftRepeat();
	virtual Boolean DPadLeftLong();
	virtual Boolean DPadLeftRelease();

	// Dir右
	virtual Boolean DPadRightPush();
	virtual Boolean DPadRightPress();
	virtual Boolean DPadRightRepeat();
	virtual Boolean DPadRightLong();
	virtual Boolean DPadRightRelease();

	// A
	virtual Boolean APush();
	virtual Boolean APress();
	virtual Boolean ARepeat();
	virtual Boolean APadALong();
	virtual Boolean APadARelease();

	// B
	virtual Boolean BPush();
	virtual Boolean BPress();
	virtual Boolean BRepeat();
	virtual Boolean BLong();
	virtual Boolean BRelease();

	// X
	virtual Boolean XPush();
	virtual Boolean XPress();
	virtual Boolean XRepeat();
	virtual Boolean XLong();
	virtual Boolean XRelease();

	// Y
	virtual Boolean YPush();
	virtual Boolean YPress();
	virtual Boolean YRepeat();
	virtual Boolean YLong();
	virtual Boolean YRelease();

	// Select
	virtual Boolean SelectPush();
	virtual Boolean SelectPress();
	virtual Boolean SelectRepeat();
	virtual Boolean SelectLong();
	virtual Boolean SelectRelease();

	// Start
	virtual Boolean StartPush();
	virtual Boolean StartPress();
	virtual Boolean StartRepeat();
	virtual Boolean StartLong();
	virtual Boolean StartRelease();

	// L1
	virtual Boolean L1Push();
	virtual Boolean L1Press();
	virtual Boolean L1Repeat();
	virtual Boolean L1Long();
	virtual Boolean L1Release();

	// R1
	virtual Boolean R1Push();
	virtual Boolean R1Press();
	virtual Boolean R1Repeat();
	virtual Boolean R1Long();
	virtual Boolean R1Release();

	// L2
	virtual Boolean L2Push();
	virtual Boolean L2Press();
	virtual Boolean L2Repeat();
	virtual Boolean L2Long();
	virtual Boolean L2Release();

	// R2
	virtual Boolean R2Push();
	virtual Boolean R2Press();
	virtual Boolean R2Repeat();
	virtual Boolean R2Long();
	virtual Boolean R2Release();

	// L3
	virtual Boolean L3Push();
	virtual Boolean L3Press();
	virtual Boolean L3Repeat();
	virtual Boolean L3Long();
	virtual Boolean L3Release();

	// R3
	virtual Boolean R3Push();
	virtual Boolean R3Press();
	virtual Boolean R3Repeat();
	virtual Boolean R3Long();
	virtual Boolean R3Release();

	// L Stick Up
	virtual Boolean LStkUpPush();
	virtual Boolean LStkUpPress();
	virtual Boolean LStkUpRepeat();
	virtual Boolean LStkUpLong();
	virtual Boolean LStkUpRelease();

	// L Stick Down
	virtual Boolean LStkDownpPush();
	virtual Boolean LStkDownpPress();
	virtual Boolean LStkDownpRepeat();
	virtual Boolean LStkDownpLong();
	virtual Boolean LStkDownpRelease();

	// L Stick left
	virtual Boolean LStkLeftPush();
	virtual Boolean LStkLeftPress();
	virtual Boolean LStkLeftRepeat();
	virtual Boolean LStkLeftLong();
	virtual Boolean LStkLeftRelease();

	// L Stick Right
	virtual Boolean LStkRightPush();
	virtual Boolean LStkRightPress();
	virtual Boolean LStkRightRepeat();
	virtual Boolean LStkRightLong();
	virtual Boolean LStkRightRelease();

	// R Stick Up
	virtual Boolean RStkUpPush();
	virtual Boolean RStkUpPress();
	virtual Boolean RStkUpRepeat();
	virtual Boolean RStkUpLong();
	virtual Boolean RStkUpRelease();

	// R Stick Down
	virtual Boolean RStkDownpPush();
	virtual Boolean RStkDownpPress();
	virtual Boolean RStkDownpRepeat();
	virtual Boolean RStkDownpLong();
	virtual Boolean RStkDownpRelease();

	// R Stick left
	virtual Boolean RStkLeftPush();
	virtual Boolean RStkLeftPress();
	virtual Boolean RStkLeftRepeat();
	virtual Boolean RStkLeftLong();
	virtual Boolean RStkLeftRelease();

	// R Stick Right
	virtual Boolean RStkRightPush();
	virtual Boolean RStkRightPress();
	virtual Boolean RStkRightRepeat();
	virtual Boolean RStkRightLong();
	virtual Boolean RStkRightRelease();

private:
	SNVirtualGamePadID ID;
};
