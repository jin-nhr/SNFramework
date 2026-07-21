#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// システムアプリ アイドル

class SNSysAppIdle : public virtual SNScene
{
public:


public:
	// コンストラクタ
	SNSysAppIdle();

	// デストラクタ
	virtual ~SNSysAppIdle();

protected:

	// Entry
	virtual Void OnEntry();

	// フレーム処理
	virtual Void OnCycle();

private:

};

