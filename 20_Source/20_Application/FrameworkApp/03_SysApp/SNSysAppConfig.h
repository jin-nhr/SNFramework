#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIText.h"
#include "SNGUITextEx.h"
#include "SNGUIButton.h"
#include "SNFocusGroup.h"

// システムアプリ コンフィグ
// 0 - Idle
// 1
// 2
// 3 - PreClose
class SNSysAppConfig : public SNGUIWindow
{
public:


public:
	// コンストラクタ
	SNSysAppConfig();

	// デストラクタ
	virtual ~SNSysAppConfig();

protected:
	virtual Void OnInitialize();
	virtual Void OnTerminate();

	virtual Void OnEntry();
	virtual Void OnExit();

	virtual Void OnCycle();

	virtual Boolean OnNotifyEvent();

private:
	static constexpr Int32 GUITextNum = 9;
	static constexpr Int32 GUITextExNum = 3;
	static constexpr Int32 GUIButtonNum = 25;

	SNGUIText GUIText[GUITextNum];
	SNGUITextEx GUITextEx[GUITextExNum];
	SNGUIButton GUIButton[GUIButtonNum];

	static SNRect GUITextRect[GUITextNum];
	static SNRect GUITextExRect[GUITextExNum];
	static SNRect GUIButtonRect[GUIButtonNum];

	static String GUITextString[GUITextNum];
	static String GUITextExString[GUITextExNum];
	static String GUIButtonString[GUIButtonNum];

	SNFocusGroup FocusGp;
};

