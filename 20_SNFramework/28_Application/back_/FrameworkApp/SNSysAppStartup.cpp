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

	LblCenter.SetText((String)L"SN Soft Studio");

	SetSceneNum(1);
	SetScene(&LblCenter);

	LblCenter.Centering(true, true);

	return;
}

