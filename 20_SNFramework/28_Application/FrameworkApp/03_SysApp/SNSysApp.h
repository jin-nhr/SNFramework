#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNSysAppStartup.h"

// システムアプリ状態制御

class SNSysApp : public SNStateController
{
public:
	SNSysApp();
	virtual ~SNSysApp();



	SNSysAppStartup Startup;
};

