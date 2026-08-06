#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

class SNSoundCodecThread : public SNThread
{
private:
	// ユーザー実行関数
	virtual Void UserMain();
};
