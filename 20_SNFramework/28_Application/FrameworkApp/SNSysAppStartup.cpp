#include "SNSysAppStartup.h"
#include "SNApplication.h"
#include "SNEvent.h"

// デバッグアプリ OFF

// コンストラクタ
SNSysAppStartup::SNSysAppStartup()
{
	return;
}

// デストラクタ
SNSysAppStartup::~SNSysAppStartup()
{
	return;
}

// 初期化
Void SNSysAppStartup::OnInitialize()
{
	LblCenter.Initialize();
	Bg.Initialize();


	LblCenter.SetText((String)L"SN Soft Studio");


	Bg.SetColor(SNColorDarkBlue);

	SetSceneNum(2);
	SetScene(&LblCenter);
	SetScene(&Bg);

	LblCenter.Centering(true, true);

	return;
}

