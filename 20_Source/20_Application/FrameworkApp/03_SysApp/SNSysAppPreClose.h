#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIWindow.h"
#include "SNGUIButton.h"
#include "SNFocusGroup.h"
#include "SNSoundEffect.h"


// アプリ終了確認
// 0 - キャンセル
// 1 - 
// 2 - 
// 3 -

class SNSysAppPreClose : public SNGUIWindow
{
public:
	// コンストラクタ
	SNSysAppPreClose();

	// デストラクタ
	virtual ~SNSysAppPreClose();

protected:

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// フレーム処理
	virtual Void OnCycle();

private:
	SNGUIText   TxtMessage;
	SNGUIButton ButtonYes;
	SNGUIButton ButtonNo;

	SNFocusGroup FocusGp;
};
