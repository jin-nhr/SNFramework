#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// システムアプリ アイドル
// 0
// 1
// 2
// 3 - PreClose
class SNSysAppIdle : public SNScene
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

