#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

// ワールドワーカースレッド

typedef Void(*SNWorldWorkerFunc)(UInt32 id);

class SNWorldWorker : public SNThread
{
public:
	SNWorldWorker();
	virtual ~SNWorldWorker();

	SNWorldWorkerFunc WorkerFunc;
	UInt32 ID;
	volatile Boolean IsComplete;

private:
	virtual Void UserMain();
};

