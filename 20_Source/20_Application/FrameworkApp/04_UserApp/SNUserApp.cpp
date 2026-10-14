#include "SNUserApp.h"
#include "SNGraphicsResManager.h"

SNUserApp::SNUserApp()
{
	SetSceneNum(6);

	//                        0   1   2   3
	SetScene(&Off,            1, -1, -1, -1);		// 0 OFF
	SetScene(&Title,          2, -1, -1, -1);		// 1 Title
	SetScene(&MenuOff,        3,  4, -1, -1);		// 2 Menu Off
	SetScene(&MainMenu,      -1, -1,  5,  2);		// 3 Main Menu
	SetScene(&CreationMenu,  -1, -1, -1,  2);		// 4 Creation Menu
	SetScene(&JumpPos,        2, -1, -1,  3);		// 5 Jump Pos

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
