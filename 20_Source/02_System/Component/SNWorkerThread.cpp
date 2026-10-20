#include "SNWorkerThread.h"




SNWorkerThread::SNWorkerThread()
{
	ID = 0;
	Param = nullptr;
	WorkerFunc = nullptr;
	IsComplete = false;
	Reset = false;

	return;
}

SNWorkerThread::~SNWorkerThread()
{
	return;
}

Void SNWorkerThread::UserMain()
{
	while (!Reset)
	{
		// Reset‚ªtrue‚É‚È‚é‚Ü‚Å‘Ò‹@‚·‚é
	}

	Reset = false;

	if (WorkerFunc != nullptr)
	{
		WorkerFunc(ID, Param);
	}
	IsComplete = true;

	return;
}
