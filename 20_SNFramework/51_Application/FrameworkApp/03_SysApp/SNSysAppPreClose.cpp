#include "SNSysAppPreClose.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"

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

	TxtMessage.SetText((String)L"しゅうりょうしますか");
	TxtMessage.Move(0, 24);
	TxtMessage.Centering(false, true);

	ButtonYes.Resize(80, 32);
	ButtonYes.Move(24, 60);
	ButtonYes.Caption.SetText((String)L"はい");
	ButtonYes.Caption.Centering(true, true);

	ButtonNo.Resize(80, 32);
	ButtonNo.Move(152, 60);
	ButtonNo.Caption.SetText((String)L"いいえ");
	ButtonNo.Caption.Centering(true, true);

	return;
}



// フレーム処理
Void SNSysAppPreClose::OnCycle()
{
	return;
}

