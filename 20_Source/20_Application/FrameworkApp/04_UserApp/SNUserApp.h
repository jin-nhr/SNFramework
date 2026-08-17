#pragma once
#include "SNFrameworkInternal.h"
#include "SNStateController.h"
#include "SNLayerController.h"
#include "SNUserAppOff.h"
#include "SNUserAppTitle.h"
#include "SNUserAppMenuOff.h"
#include "SNUserAppMainMenu.h"
#include "SNWorldApp.h"

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
	SNLayerController Run;
	SNStateController Menu;
	SNUserAppMenuOff MenuOff;
	SNUserAppMainMenu MainMenu;
	SNWorldApp World;
};

