#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

// ワールドワーカースレッド

typedef Void(*SNWorkerThreadFunc)(UInt32 id, Void* param);

class SNWorkerThread : public SNThread
{
public:
	SNWorkerThread();
	virtual ~SNWorkerThread();

	SNWorkerThreadFunc WorkerFunc;
	UInt32 ID;
	Void* Param;
	volatile Boolean IsComplete;
	volatile Boolean Reset;

private:
	virtual Void UserMain();
};

