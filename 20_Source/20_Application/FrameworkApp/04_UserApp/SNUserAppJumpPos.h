#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIButton.h"
#include "SNGUITextEx.h"
#include "SNFocusGroup.h"

// ジャンプ
// 0 - 
// 1 - 
// 2 -
// 3 - キャンセル

class SNUserAppJumpPos : public SNGUIWindow
{
public:
	SNUserAppJumpPos();
	virtual ~SNUserAppJumpPos();

	static SNWorldPos* GetMoveToPos();

protected:
	virtual Void OnInitialize();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Void OnCycle();

	virtual Void OnPreDraw();

protected:
	enum SNJumpMenuText
	{
		SNJumpMenuX,
		SNJumpMenuY,
		SNJumpMenuZ,
		SNJumpMenuTextNum
	};

	enum SNJumpMenuTextEx
	{
		SNJumpMenuXPM,
		SNJumpMenuX10000,
		SNJumpMenuX1000,
		SNJumpMenuX100,
		SNJumpMenuX10,
		SNJumpMenuX1,
		SNJumpMenuYPM,
		SNJumpMenuY10000,
		SNJumpMenuY1000,
		SNJumpMenuY100,
		SNJumpMenuY10,
		SNJumpMenuY1,
		SNJumpMenuZPM,
		SNJumpMenuZ10000,
		SNJumpMenuZ1000,
		SNJumpMenuZ100,
		SNJumpMenuZ10,
		SNJumpMenuZ1,
		SNJumpMenuTextExNum
	};
	enum SNJumpMenuButton
	{
		SNJumpMenuButtonXP,
		SNJumpMenuButtonXP10000,
		SNJumpMenuButtonXP1000,
		SNJumpMenuButtonXP100,
		SNJumpMenuButtonXP10,
		SNJumpMenuButtonXP1,
		SNJumpMenuButtonXM,
		SNJumpMenuButtonXM10000,
		SNJumpMenuButtonXM1000,
		SNJumpMenuButtonXM100,
		SNJumpMenuButtonXM10,
		SNJumpMenuButtonXM1,
		SNJumpMenuButtonYP,
		SNJumpMenuButtonYP10000,
		SNJumpMenuButtonYP1000,
		SNJumpMenuButtonYP100,
		SNJumpMenuButtonYP10,
		SNJumpMenuButtonYP1,
		SNJumpMenuButtonYM,
		SNJumpMenuButtonYM10000,
		SNJumpMenuButtonYM1000,
		SNJumpMenuButtonYM100,
		SNJumpMenuButtonYM10,
		SNJumpMenuButtonYM1,
		SNJumpMenuButtonZP,
		SNJumpMenuButtonZP10000,
		SNJumpMenuButtonZP1000,
		SNJumpMenuButtonZP100,
		SNJumpMenuButtonZP10,
		SNJumpMenuButtonZP1,
		SNJumpMenuButtonZM,
		SNJumpMenuButtonZM10000,
		SNJumpMenuButtonZM1000,
		SNJumpMenuButtonZM100,
		SNJumpMenuButtonZM10,
		SNJumpMenuButtonZM1,
		SNJumpMenuButtonOK,
		SNJumpMenuButtonCancel,
		SNJumpMenuButtonNum
	};

	static SNRect TextRect[SNJumpMenuTextNum];
	static String TextCaption[SNJumpMenuTextNum];
	SNGUIText Text[SNJumpMenuTextNum];

	static SNRect TextExRect[SNJumpMenuTextExNum];
	static String TextExCaption[SNJumpMenuTextExNum];
	SNGUITextEx TextEx[SNJumpMenuTextExNum];

	static SNRect ButtonRect[SNJumpMenuButtonNum];
	static String ButtonCaption[SNJumpMenuButtonNum];
	SNGUIButton Button[SNJumpMenuButtonNum];

	SNFocusGroup FocusGp;

	Int64 Xpm;
	Int64 X10000;
	Int64 X1000;
	Int64 X100;
	Int64 X10;
	Int64 X1;

	Int64 Ypm;
	Int64 Y10000;
	Int64 Y1000;
	Int64 Y100;
	Int64 Y10;
	Int64 Y1;

	Int64 Zpm;
	Int64 Z10000;
	Int64 Z1000;
	Int64 Z100;
	Int64 Z10;
	Int64 Z1;

	static SNWorldPos MoveToPos;
};

