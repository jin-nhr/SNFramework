#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

class SNUserAppOff : public SNScene
{
public:
	SNUserAppOff();
	virtual ~SNUserAppOff();

	virtual Boolean OnInternalEvent();

};

