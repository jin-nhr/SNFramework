#include "SNUserAppJumpPos.h"
#include "SNFixedString.h"
#include "SNFocus.h"
#include "SNVirtualGamePad.h"
#include "SNVirtualPointingDevice.h"
#include "SNMath.h"
#include "SNWorld.h"

SNRect SNUserAppJumpPos::TextRect[SNJumpMenuTextNum] =
{
	32, 68, 32, 24,
	260 + 32, 68, 32, 24,
	524 + 32, 68, 32, 24,
};

String SNUserAppJumpPos::TextCaption[SNJumpMenuTextNum] =
{
	(String)L"X=",
	(String)L"Y=",
	(String)L"Z=",
};

SNRect SNUserAppJumpPos::TextExRect[SNJumpMenuTextExNum] =
{
	72, 68, 16, 24,
	72 + 36, 68, 16, 24,
	72 + 72, 68, 16, 24,
	72 + 108, 68, 16, 24,
	72 + 144, 68, 16, 24,
	72 + 180, 68, 16, 24,

	260 + 72, 68, 16, 24,
	260 + 72 + 36, 68, 16, 24,
	260 + 72 + 72, 68, 16, 24,
	260 + 72 + 108, 68, 16, 24,
	260 + 72 + 144, 68, 16, 24,
	260 + 72 + 180, 68, 16, 24,

	524 + 72, 68, 16 ,24,
	524 + 72 + 36, 68, 16, 24,
	524 + 72 + 72, 68, 16, 24,
	524 + 72 + 108, 68, 16, 24,
	524 + 72 + 144, 68, 16, 24,
	524 + 72 + 180, 68, 16, 24,
};

String SNUserAppJumpPos::TextExCaption[SNJumpMenuTextExNum] =
{
	(String)L"+",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",

	(String)L"+",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",

	(String)L"+",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
	(String)L"%1d",
};

SNRect SNUserAppJumpPos::ButtonRect[SNJumpMenuButtonNum] =
{

	{64, 32, 32, 32},
	{100, 32, 32, 32},
	{136, 32, 32, 32},
	{172, 32, 32, 32},
	{208, 32, 32, 32},
	{244, 32, 32, 32},

	{64, 96, 32, 32},
	{100, 96, 32, 32},
	{136, 96, 32, 32},
	{172, 96, 32, 32},
	{208, 96, 32, 32},
	{244, 96, 32, 32},


	{260 +64, 32, 32, 32},
	{260 + 100, 32, 32, 32},
	{260 + 136, 32, 32, 32},
	{260 + 172, 32, 32, 32},
	{260 + 208, 32, 32, 32},
	{260 + 244, 32, 32, 32},

	{260 + 64, 96, 32, 32},
	{260 + 100, 96, 32, 32},
	{260 + 136, 96, 32, 32},
	{260 + 172, 96, 32, 32},
	{260 + 208, 96, 32, 32},
	{260 + 244, 96, 32, 32},


	{524 + 64, 32, 32, 32},
	{524 + 100, 32, 32, 32},
	{524 + 136, 32, 32, 32},
	{524 + 172, 32, 32, 32},
	{524 + 208, 32, 32, 32},
	{524 + 244, 32, 32, 32},

	{524 + 64, 96, 32, 32},
	{524 + 100, 96, 32, 32},
	{524 + 136, 96, 32, 32},
	{524 + 172, 96, 32, 32},
	{524 + 208, 96, 32, 32},
	{524 + 244, 96, 32, 32},

	{208 + 64, 144, 120, 32},
	{224 + 64 + 184, 144, 120, 32},
};

String SNUserAppJumpPos::ButtonCaption[SNJumpMenuButtonNum] =
{
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpPlus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::JumpMinus,
	SNFixedString::ConfigOK,
	SNFixedString::ConfigCancel,
};

SNWorldPos SNUserAppJumpPos::MoveToPos;


SNUserAppJumpPos::SNUserAppJumpPos()
{
	Xpm = 1;
	X10000 = 0;
	X1000 = 0;
	X100 = 0;
	X10 = 0;
	X1 = 0;

	Ypm = 1;
	Y10000 = 0;
	Y1000 = 0;
	Y100 = 0;
	Y10 = 0;
	Y1 = 0;

	Zpm = 1;
	Z10000 = 0;
	Z1000 = 0;
	Z100 = 0;
	Z10 = 0;
	Z1 = 0;

	MoveToPos.X = 0;
	MoveToPos.Y = 0;
	MoveToPos.Z = 0;

	return;
}

SNUserAppJumpPos::~SNUserAppJumpPos()
{
	return;
}

SNWorldPos* SNUserAppJumpPos::GetMoveToPos()
{
	return &MoveToPos;
}

Void SNUserAppJumpPos::OnInitialize()
{
	Int32 cnt;

	SetRect(32, 32, 844, 208);
	Centering(true, true);

	SetSceneNum(SNJumpMenuTextNum + SNJumpMenuTextExNum + SNJumpMenuButtonNum);

	for (cnt = 0; cnt < SNJumpMenuTextNum; cnt++)
	{
		SetScene(&Text[cnt]);
		Text[cnt].SetRect(
			TextRect[cnt].PointX,
			TextRect[cnt].PointY,
			TextRect[cnt].Width,
			TextRect[cnt].Height);
		Text[cnt].SetText(TextCaption[cnt]);
	}

	for (cnt = 0; cnt < SNJumpMenuTextExNum; cnt++)
	{
		SetScene(&TextEx[cnt]);
		TextEx[cnt].SetRect(
			TextExRect[cnt].PointX,
			TextExRect[cnt].PointY,
			TextExRect[cnt].Width,
			TextExRect[cnt].Height);
		TextEx[cnt].SetText(TextExCaption[cnt]);
	}

	FocusGp.StartRegister(SNFocusLayerUserApp, SNJumpMenuButtonNum, true, false);

	for (cnt = 0; cnt < SNJumpMenuButtonNum; cnt++)
	{
		SetScene(&Button[cnt]);
		FocusGp.RegisterButton(&Button[cnt]);
		Button[cnt].SetRect(
			ButtonRect[cnt].PointX,
			ButtonRect[cnt].PointY,
			ButtonRect[cnt].Width,
			ButtonRect[cnt].Height);
		Button[cnt].Caption.SetText(ButtonCaption[cnt]);
		Button[cnt].Caption.Centering(true, true);
	}

	FocusGp.EndRegister();

	return;
}

Void SNUserAppJumpPos::OnEntry()
{
	SNWorldPos* current = SNWorld::GetCurrentPos();
	Int32 x = (Int32)current->X;
	Int32 y = (Int32)current->Y;
	Int32 z = (Int32)current->Z;

	FocusGp.Entry();

	if (x >= 0)
	{
		Xpm = 1;
	}
	else
	{
		Xpm = -1;
	}

	x = (Int32)SNMath::AbsF((Float32)x);

	x = x % 100000;
	X10000 = x / 10000;
	x = x % 10000;
	X1000 = x / 1000;
	x = x % 1000;
	X100 = x / 100;
	x = x % 100;
	X10 = x / 10;
	x = x % 10;
	X1 = x;

	if (y >= 0)
	{
		Ypm = 1;
	}
	else
	{
		Ypm = -1;
	}

	y = (Int32)SNMath::AbsF((Float32)y);

	y = y % 100000;
	Y10000 = y / 10000;
	y = y % 10000;
	Y1000 = y / 1000;
	y = y % 1000;
	Y100 = y / 100;
	y = y % 100;
	Y10 = y / 10;
	y = y % 10;
	Y1 = y;

	if (z >= 0)
	{
		Zpm = 1;
	}
	else
	{
		Zpm = -1;
	}

	z = (Int32)SNMath::AbsF((Float32)z);

	z = z % 100000;
	Z10000 = z / 10000;
	z = z % 10000;
	Z1000 = z / 1000;
	z = z % 1000;
	Z100 = z / 100;
	z = z % 100;
	Z10 = z / 10;
	z = z % 10;
	Z1 = z;	

	return;
}

Void SNUserAppJumpPos::OnExit()
{
	FocusGp.Exit();

	return;
}

Void SNUserAppJumpPos::OnCycle()
{
	// X+
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP]))
	{
		Xpm = 1;
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP10000]))
	{
		X10000 =SNMath::Increment(X10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP1000]))
	{
		X1000 = SNMath::Increment(X1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP100]))
	{
		X100 = SNMath::Increment(X100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP10]))
	{
		X10 = SNMath::Increment(X10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXP1]))
	{
		X1 = SNMath::Increment(X1, 0, 9);
	}

	// X-
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM]))
	{
		Xpm = -1;
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM10000]))
	{
		X10000 = SNMath::Decrement(X10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM1000]))
	{
		X1000 = SNMath::Decrement(X1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM100]))
	{
		X100 = SNMath::Decrement(X100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM10]))
	{
		X10 = SNMath::Decrement(X10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonXM1]))
	{
		X1 = SNMath::Decrement(X1, 0, 9);
	}

	// Y+
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP]))
	{
		Ypm = 1;
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP10000]))
	{
		Y10000 = SNMath::Increment(Y10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP1000]))
	{
		Y1000 = SNMath::Increment(Y1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP100]))
	{
		Y100 = SNMath::Increment(Y100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP10]))
	{
		Y10 = SNMath::Increment(Y10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYP1]))
	{
		Y1 = SNMath::Increment(Y1, 0, 9);
	}

	// Y-
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM]))
	{
		Ypm = -1;
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM10000]))
	{
		Y10000 = SNMath::Decrement(Y10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM1000]))
	{
		Y1000 = SNMath::Decrement(Y1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM100]))
	{
		Y100 = SNMath::Decrement(Y100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM10]))
	{
		Y10 = SNMath::Decrement(Y10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonYM1]))
	{
		Y1 = SNMath::Decrement(Y1, 0, 9);
	}


	// Z+
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP]))
	{
		Zpm = 1;
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP10000]))
	{
		Z10000 = SNMath::Increment(Z10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP1000]))
	{
		Z1000 = SNMath::Increment(Z1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP100]))
	{
		Z100 = SNMath::Increment(Z100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP10]))
	{
		Z10 = SNMath::Increment(Z10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZP1]))
	{
		Z1 = SNMath::Increment(Z1, 0, 9);
	}

	// Z-
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM]))
	{
		Zpm = -1;
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM10000]))
	{
		Z10000 = SNMath::Decrement(Z10000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM1000]))
	{
		Z1000 = SNMath::Decrement(Z1000, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM100]))
	{
		Z100 = SNMath::Decrement(Z100, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM10]))
	{
		Z10 = SNMath::Decrement(Z10, 0, 9);
	}
	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonZM1]))
	{
		Z1 = SNMath::Decrement(Z1, 0, 9);
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonOK]))
	{
		MoveToPos.X = (Float32)(Xpm * (X10000 * 10000 + X1000 * 1000 + X100 * 100 + X10 * 10 + X1));
		MoveToPos.Y = (Float32)(Ypm * (Y10000 * 10000 + Y1000 * 1000 + Y100 * 100 + Y10 * 10 + Y1));
		MoveToPos.Z = (Float32)(Zpm * (Z10000 * 10000 + Z1000 * 1000 + Z100 * 100 + Z10 * 10 + Z1));

		SNEvent::EventResult[SNEventResultMenuJumpPos] = true;
		TransCode = SNTransitionCode0;
	}

	if (FocusGp.JudgeActDecide(&Button[SNJumpMenuButtonCancel]))
	{
		TransCode = SNTransitionCode3;
	}

	// ƒLƒƒƒ“ƒZƒ‹‘€ì
	if (FocusGp.JudgeActCancel())
	{
		SNFocus::CallbackPushButton();
		TransCode = SNTransitionCode3;
	}

	return;
}

Void SNUserAppJumpPos::OnPreDraw()
{
	if (Xpm >= 0)
	{
		TextEx[SNJumpMenuXPM].SetText((SNFixedString::JumpPlus));
	}
	else
	{
		TextEx[SNJumpMenuXPM].SetText((SNFixedString::JumpMinus));
	}

	if (Ypm >= 0)
	{
		TextEx[SNJumpMenuYPM].SetText((SNFixedString::JumpPlus));
	}
	else
	{
		TextEx[SNJumpMenuYPM].SetText((SNFixedString::JumpMinus));
	}

	if (Zpm >= 0)
	{
		TextEx[SNJumpMenuZPM].SetText((SNFixedString::JumpPlus));
	}
	else
	{
		TextEx[SNJumpMenuZPM].SetText((SNFixedString::JumpMinus));
	}

	TextEx[SNJumpMenuX10000].SetValue(X10000);
	TextEx[SNJumpMenuX1000].SetValue(X1000);
	TextEx[SNJumpMenuX100].SetValue(X100);
	TextEx[SNJumpMenuX10].SetValue(X10);
	TextEx[SNJumpMenuX1].SetValue(X1);

	TextEx[SNJumpMenuY10000].SetValue(Y10000);
	TextEx[SNJumpMenuY1000].SetValue(Y1000);
	TextEx[SNJumpMenuY100].SetValue(Y100);
	TextEx[SNJumpMenuY10].SetValue(Y10);
	TextEx[SNJumpMenuY1].SetValue(Y1);

	TextEx[SNJumpMenuZ10000].SetValue(Z10000);
	TextEx[SNJumpMenuZ1000].SetValue(Z1000);
	TextEx[SNJumpMenuZ100].SetValue(Z100);
	TextEx[SNJumpMenuZ10].SetValue(Z10);
	TextEx[SNJumpMenuZ1].SetValue(Z1);

	return;
}
