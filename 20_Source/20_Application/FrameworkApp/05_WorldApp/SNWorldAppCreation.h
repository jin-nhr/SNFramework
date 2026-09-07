#pragma once
#include "SNFrameworkInternal.h"
#include "SNWorldAppBase.h"

class SNWorldAppCreation : public SNWorldAppBase
{
public:
	static constexpr Int32 MapchipListView = 4;
public:
	SNWorldAppCreation();
	virtual ~SNWorldAppCreation();

	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Boolean OnInternalEvent();

	virtual Void OnPreDraw();

	virtual Void OnDraw(SNGraphicsContext* grc);

private:
	SNGUIWindow Win;
	SNGUITextEx	txtX;
	SNGUITextEx txtY;
	SNGUITextEx txtZ;
	SNGUIWindow WinBlock;

	UInt16 SelectBlock;
};

