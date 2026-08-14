#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGUIPicture.h"
#include "SNGUIWindow.h"
#include "SNGUIButton.h"
#include "SNTimerSequencer.h"
#include "SNFocusGroup.h"
#include "SNMusic.h"

// システムアプリ タイトル
// 0 - はじめから
// 1 - つづきから
// 2 - システム設定
// 3 - アプリ終了

class SNSysAppTitle : public SNLayerController
{
public:
	// コンストラクタ
	SNSysAppTitle();

	// デストラクタ
	virtual ~SNSysAppTitle();

protected:
	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Boolean OnNotifyEvent();

	virtual Void OnCycle();

	// シーケンサ用コールバック
	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

private:
	enum GUIButtonObj
	{
		GUIButtonNew,
		GUIButtonLoad,
		GUIButtonConfig,
		GUIButtonExit,
		GUIButtonNum
	};

	static SNRect GUIButtonRect[GUIButtonNum];
	static String GUIButtonString[GUIButtonNum];

	SNGUIWindow GUIWin;
	SNGUIButton GUIButton[GUIButtonNum];

	SNTimerSequencer	 Seq;
	SNFocusGroup FocusGp;
};

