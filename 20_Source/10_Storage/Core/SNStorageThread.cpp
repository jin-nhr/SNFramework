#include "SNStorageThread.h"
#include "SNStorage.h"


// ストレージスレッドメイン
Void SNStorageThread::UserMain()
{
	// ストレージクラスのメイン実行
	SNStorage::UserMain();

	return;
}

