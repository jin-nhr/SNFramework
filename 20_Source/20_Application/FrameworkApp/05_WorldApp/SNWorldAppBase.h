#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUIWorldView.h"
#include "SNGUITextEx.h"
#include "SNGUIWindow.h"
#include "SNMapchip.h"

class SNWorldAppBase : public SNScene
{
public:
	SNWorldAppBase();
	virtual ~SNWorldAppBase();

	virtual Void OnInitialize();

	virtual Void OnTerminate();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Boolean OnInternalEvent();

	virtual Void OnPreDraw();

	virtual Void OnDraw(SNGraphicsContext* grc);

protected:
	SNGUIWorldView WorldView;
};

