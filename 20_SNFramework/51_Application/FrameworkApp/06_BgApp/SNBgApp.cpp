#include "SNBgApp.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNUserConfig.h"

// バックグラウンドアプリ

// コンストラクタ
SNBgApp::SNBgApp()
{
	return;
}

// デストラクタ
SNBgApp::~SNBgApp()
{
	return;
}

// アプリ通知
Boolean SNBgApp::OnNotifyEvent()
{
	Boolean ret = false;

	// 終了通知
	if (SNApplication::GetEventInfo(SNEventExitApplication))
	{
		SNEvent::EventResult[SNEventResultExitApplication] = true;
		ret = true;
	}

	// 最大化
	if (SNApplication::GetEventInfo(SNEventMaximizeDisp))
	{
		SNUserConfig::Data.FullScreen = true;
		ret = true;
	}

	// 通常表示
	if (SNApplication::GetEventInfo(SNEventNormalDisp))
	{
		SNUserConfig::Data.FullScreen = false;
		ret = true;
	}

	return ret;
}
