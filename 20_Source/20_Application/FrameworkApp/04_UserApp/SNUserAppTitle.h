#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIPicture.h"


class SNUserAppTitle : public SNGUIPicture
{
public:
	SNUserAppTitle();
	virtual ~SNUserAppTitle();

	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnInternalEvent();

	virtual Void OnPreDraw();
};

