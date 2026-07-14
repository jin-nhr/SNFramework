#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

class SNImageCodecThread : public SNThread
{
private:
	// ユーザー実行関数
	virtual Void UserMain();
};
