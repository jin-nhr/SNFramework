#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"


// ストレージスレッド
class SNStorageThread : public SNThread
{
private:
	// スレッドクラスのユーザー実行関数
	Void UserMain();
};
