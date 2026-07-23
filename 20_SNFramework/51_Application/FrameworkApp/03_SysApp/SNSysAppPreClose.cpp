#include "SNSysAppPreClose.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFixedString.h"

// コンストラクタ
SNSysAppPreClose::SNSysAppPreClose()
{
	Size.Width = 256;
	Size.Height = 128;

	SetSceneNum(3);
	SetScene(&ButtonYes);
	SetScene(&ButtonNo);
	SetScene(&TxtMessage);

	return;
}

// デストラクタ
SNSysAppPreClose::~SNSysAppPreClose()
{
	return;
}

// Entry
Void SNSysAppPreClose::OnEntry()
{
	Centering(true, true);

	TxtMessage.SetText(SNFixedString::PreClose1);
	TxtMessage.Move(0, 24);
	TxtMessage.Centering(false, true);

	ButtonYes.Resize(80, 32);
	ButtonYes.Move(24, 60);
	ButtonYes.Caption.SetText(SNFixedString::PreClose2);
	ButtonYes.Caption.Centering(true, true);

	ButtonNo.Resize(80, 32);
	ButtonNo.Move(152, 60);
	ButtonNo.Caption.SetText(SNFixedString::PreClose3);
	ButtonNo.Caption.Centering(true, true);

	return;
}



// フレーム処理
Void SNSysAppPreClose::OnCycle()
{
	return;
}

