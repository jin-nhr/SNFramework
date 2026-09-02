#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

class SNWorldAppOff : public SNScene
{
public:
	SNWorldAppOff();
	virtual ~SNWorldAppOff();

protected:
	virtual Boolean OnInternalEvent();

private:


};

