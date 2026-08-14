#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"



class SNUserAppIdle : public SNScene
{
public:
	SNUserAppIdle();
	virtual ~SNUserAppIdle();

	virtual Void OnEntry();

};

