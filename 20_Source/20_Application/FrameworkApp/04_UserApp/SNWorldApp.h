#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUIWorldView.h"
#include "SNGUITextEx.h"
#include "SNGUIWindow.h"
#include "SNMapchip.h"

class SNWorldApp : public SNScene
{
public:
	SNWorldApp();
	virtual ~SNWorldApp();

	virtual Void OnInitialize();

	virtual Void OnEntry();

	virtual Void OnExit();

	virtual Boolean OnGamePad1();

	virtual Void OnPreDraw();

	virtual Void OnDraw(SNGraphicsContext* grc);

private:
	SNGUIWorldView WorldView;
	SNGUIWindow Win;
	SNGUITextEx	txtX;
	SNGUITextEx txtY;
	SNGUITextEx txtZ;
	SNGUIWindow WinBlock;

	UInt16 SelectBlock;
};

