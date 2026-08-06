#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

// サウンドスレッド
class SNSoundThread : public SNThread
{
public:
	// ユーザー実行関数
	Void UserMain();
};

