#include "SNBgApp.h"
#include "SNApplication.h"
#include "SNEvent.h"

// バックグラウンドレイヤクラス

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

	// 終了通知のみ拾って処理する

	if (SNApplication::GetEventInfo(SNEventExitApplication))
	{
		SNEvent::EventResult[SNEventResultExitApplication] = true;
		ret = true;
	}

	return ret;
}
