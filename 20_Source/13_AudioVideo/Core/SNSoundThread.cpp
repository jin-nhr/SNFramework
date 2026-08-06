#include "SNSoundThread.h"
#include "SNSoundDevice.h"


//スレッドメイン
Void SNSoundThread::UserMain()
{
	// サウンドクラスのメイン実行
	SNSoundDevice::UserMain();

	return;
}

