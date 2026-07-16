#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// バックグラウンドアプリ
class SNBgApp : public virtual SNScene
{
public:
	// コンストラクタ
	SNBgApp();

	// デストラクタ
	virtual ~SNBgApp();

protected:
	// アプリ通知
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnNotifyEvent();

private:

};

