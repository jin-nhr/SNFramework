#include "SNBackgroundLayer.h"
#include "SNApplication.h"
#include "SNEvent.h"

// バックグラウンドレイヤクラス

// コンストラクタ
SNBackgroundLayer::SNBackgroundLayer()
{
	return;
}

// デストラクタ
SNBackgroundLayer::~SNBackgroundLayer()
{
	return;
}

// アプリ通知
Boolean SNBackgroundLayer::OnNotifyEvent()
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
