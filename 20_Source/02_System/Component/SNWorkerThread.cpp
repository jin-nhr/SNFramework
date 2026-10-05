#include "SNWorkerThread.h"




SNWorkerThread::SNWorkerThread()
{
	ID = 0;
	Param = nullptr;
	WorkerFunc = nullptr;
	IsComplete = false;

	return;
}

SNWorkerThread::~SNWorkerThread()
{
	return;
}

Void SNWorkerThread::UserMain()
{
	if (WorkerFunc != nullptr)
	{
		WorkerFunc(ID, Param);
	}
	IsComplete = true;

	return;
}
