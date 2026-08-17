#include "SNUserApp.h"
#include "SNGraphicsResManager.h"

SNUserApp::SNUserApp()
{
	SetSceneNum(3);

	//                  0   1   2   3
	SetScene(&Off, 1, -1, -1, -1);		// 0 OFF
	SetScene(&Title, 2, -1, -1, -1);		// 1 Title
	SetScene(&Run, -1, -1, -1, -1);		// 2 Run


	Run.SetSceneNum(2);
	Run.SetScene(&Menu);
	Run.SetScene(&World);


	Menu.SetSceneNum(2);
	//							 0   1   2   3
	Menu.SetScene(&MenuOff, 1, -1, -1, -1);		// 0 Off
	Menu.SetScene(&MainMenu, -1, -1, -1, 0);		// 1 Main Menu

	return;
}

SNUserApp::~SNUserApp()
{
	return;
}

Void SNUserApp::OnInitialize()
{

	return;
}


Void SNUserApp::OnTerminate()
{
	return;
}
