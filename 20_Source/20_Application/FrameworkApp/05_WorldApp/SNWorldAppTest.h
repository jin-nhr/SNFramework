#pragma once
#include "SNFrameworkInternal.h"
#include "SNWorldAppBase.h"

class SNWorldAppTest : public SNWorldAppBase
{
public:
	SNWorldAppTest();
	virtual ~SNWorldAppTest();

	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Boolean OnInternalEvent();

	virtual Void OnCycle();

	virtual Void OnPreDraw();

	virtual Void OnDraw(SNGraphicsContext* grc);

private:
	SNGUIWindow Win;
	SNGUITextEx	txtX;
	SNGUITextEx txtY;
	SNGUITextEx txtZ;
};

