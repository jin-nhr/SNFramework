#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNSysAppStartup.h"
#include "SNSysAppLoading.h"
#include "SNSysAppError.h"
#include "SNSysAppIdle.h"
#include "SNSysAppPreClose.h"
#include "SNSysAppConfig.h"
#include "SNSysAppTitle.h"

// システムアプリ状態制御

class SNSysApp : public SNStateController
{
public:
	SNSysApp();
	virtual ~SNSysApp();

	SNSysAppStartup Startup;
	SNSysAppLoading Loading;
	SNSysAppError Error;
	SNSysAppTitle Title;
	SNSysAppPreClose PreClose;
	SNSysAppConfig Config;
	SNSysAppIdle Idle;
};

