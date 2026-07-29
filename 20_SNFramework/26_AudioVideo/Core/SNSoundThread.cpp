#include "SNSoundThread.h"
#include "SNSound.h"


//スレッドメイン
Void SNSoundThread::UserMain()
{
	// サウンドクラスのメイン実行
	SNSound::UserMain();

	return;
}

