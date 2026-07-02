#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// バックグラウンドレイヤクラス
class SNBackgroundLayer : public virtual SNScene
{
public:
	// コンストラクタ
	SNBackgroundLayer();

	// デストラクタ
	virtual ~SNBackgroundLayer();

protected:
	// アプリ通知
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnNotifyEvent();

private:

};

