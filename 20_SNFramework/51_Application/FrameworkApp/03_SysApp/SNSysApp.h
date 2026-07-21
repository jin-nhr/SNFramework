#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNSysAppStartup.h"
#include "SNSysAppLoading.h"
#include "SNSysAppError.h"
#include "SNSysAppIdle.h"

// システムアプリ状態制御

class SNSysApp : public SNStateController
{
public:
	SNSysApp();
	virtual ~SNSysApp();



	SNSysAppStartup Startup;
	SNSysAppLoading Loading;
	SNSysAppError Error;
	SNSysAppIdle Idle;
};

