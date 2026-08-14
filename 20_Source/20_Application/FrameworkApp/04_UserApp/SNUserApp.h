#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNUserAppOff.h"
#include "SNUserAppTitle.h"
#include "SNUserAppIdle.h"


class SNUserApp : public SNStateController
{
public:
	SNUserApp();
	virtual ~SNUserApp();

	virtual Void OnInitialize();

	virtual Void OnTerminate();


private:
	SNUserAppOff Off;
	SNUserAppTitle Title;
	SNUserAppIdle Idle;

};

