#include "SNWorldWorker.h"




SNWorldWorker::SNWorldWorker()
{
	ID = 0;
	WorkerFunc = nullptr;
	IsComplete = false;

	return;
}

SNWorldWorker::~SNWorldWorker()
{
	return;
}

Void SNWorldWorker::UserMain()
{
	if (WorkerFunc != nullptr)
	{
		WorkerFunc(ID);
	}
	IsComplete = true;

	return;
}
