#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNWorldAppCreation.h"
#include "SNWorldAppOff.h"
#include "SNWorldAppTest.h"

class SNWorldApp : public SNStateController
{
public:
	SNWorldApp();
	virtual ~SNWorldApp();

	virtual Void OnInitialize();

	virtual Void OnTerminate();


private:
	SNWorldAppOff Off;
	SNWorldAppCreation Creation;
	SNWorldAppTest Test;
};

