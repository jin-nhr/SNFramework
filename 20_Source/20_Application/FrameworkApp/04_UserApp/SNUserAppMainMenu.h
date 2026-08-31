#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIButton.h"
#include "SNFocusGroup.h"

// メインメニュー
// 0 - 
// 1 - 
// 2 -
// 3 - キャンセル

class SNUserAppMainMenu : public SNGUIWindow
{
public:
	SNUserAppMainMenu();
	virtual ~SNUserAppMainMenu();

protected:
	virtual Void OnInitialize();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Void OnCycle();

protected:
	enum SNMainMenuButton
	{
		SNMainMenuButtonMap,
		SNMainMenuButtonPreset,
		SNMainMenuButtonStaticObj,
		SNMainMenuButtonActiveObj,
		SNMainMenuButtonTestPlay,
		SNMainMenuButtonConfig,
		SNMainMenuButtonExit,
		SNMainMenuButtonNum
	};

	static SNRect ButtonRect[SNMainMenuButtonNum];
	static String ButtonCaption[SNMainMenuButtonNum];
	SNGUIButton Button[SNMainMenuButtonNum];

	SNFocusGroup FocusGp;
};

