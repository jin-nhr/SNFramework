#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// システムアプリ アイドル
// 0
// 1
// 2
// 3 - アプリ終了

class SNSysAppIdle : public SNScene
{
public:


public:
	// コンストラクタ
	SNSysAppIdle();

	// デストラクタ
	virtual ~SNSysAppIdle();

protected:
	virtual Void OnEntry();

	virtual Boolean OnNotifyEvent();

	virtual Boolean OnInternalEvent();

private:

};

