#include "SNUserApp.h"
#include "SNGraphicsResManager.h"

SNUserApp::SNUserApp()
{
	SetSceneNum(4);

	//                  0   1   2   3
	SetScene(&Off,      1, -1, -1, -1);		// 0 OFF
	SetScene(&Title,    2, -1, -1, -1);		// 1 Title
	SetScene(&MenuOff,  3, -1, -1, -1);		// 2 Menu Off
	SetScene(&MainMenu,-1, -1, -1,  2);		// 3 Main Menu

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
