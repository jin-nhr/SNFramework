#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIText.h"
#include "SNGUITextEx.h"
#include "SNGUIButton.h"
#include "SNFocusGroup.h"

// システムアプリ コンフィグ
// 0 - クローズ
// 1
// 2
// 3 - アプリ終了
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

	virtual Void UpdateState();
	virtual Void ProcButton();

private:
	enum GUITextObj
	{
		GUITextTitle,
		GUITextScreenMode,
		GUITextVSYNC,
		GUITextDrawFilter,
		GUITextMasterVolume,
		GUITextBGMVolume,
		GUITextSEVolume,
		GUITextPlayer1Input,
		GUITextPlayer2Input,
		GUITextNum
	};

	enum GUITexxtExObj
	{
		GUITextExMasterVolume,
		GUITextExBGMVolume,
		GUITextExSEVolume,
		GUITextExNum
	};

	enum GUIButtonObj
	{
		GUIButtonWindow,
		GUIButtonFullScreen,
		GUIButtonVSYNCOFF,
		GUIButtonVSYNCON,
		GUIButtonFilterOFF,
		GUIButtonFilterON,
		GUIButtonMasterVolDown10,
		GUIButtonMasterVolDown1,
		GUIButtonMasterVolUp1,
		GUIButtonMasterVolUp10,
		GUIButtonBGMVolDown10,
		GUIButtonBGMVolDown1,
		GUIButtonBGMVolUp1,
		GUIButtonBGMVolUp10,
		GUIButtonSEVolDown10,
		GUIButtonSEVolDown1,
		GUIButtonSEVolUp1,
		GUIButtonSEVolUp10,
		GUIButtonP1GamePad,
		GUIButtonP1Keyboard,
		GUIButtonP2GamePad,
		GUIButtonP2Keyboard,
		GUIButtonInitialize,
		GUIButtonOK,
		GUIButtonCancel,
		GUIButtonNum
	};

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

