#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIButton.h"
#include "SNFocusGroup.h"

// マップ作成メニュー
// 0 - 
// 1 - 
// 2 -
// 3 - キャンセル

class SNUserAppCreationMenu : public SNGUIWindow
{
public:
	SNUserAppCreationMenu();
	virtual ~SNUserAppCreationMenu();

protected:
	virtual Void OnInitialize();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Void OnCycle();

protected:
	enum SNMainMenuButton
	{
		SNCreationMenuButtonCopy,
		SNCreationMenuButtonCut,
		SNCreationMenuButtonRotateL,
		SNCreationMenuButtonRotateR,
		SNCreationMenuButtonDeselect,
		SNCreationMenuButtonNum
	};

	static SNRect ButtonRect[SNCreationMenuButtonNum];
	static String ButtonCaption[SNCreationMenuButtonNum];
	SNGUIButton Button[SNCreationMenuButtonNum];

	SNFocusGroup FocusGp;
};

