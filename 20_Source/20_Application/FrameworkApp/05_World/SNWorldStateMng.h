#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNWorldOff.h"
#include "SNWorld.h"

class SNWorldStateMng : public SNStateController
{
public:
	virtual Void OnInitialize();
	virtual Void OnTerminate();


private:
	SNWorldOff Off;
	SNWorld World;
};

