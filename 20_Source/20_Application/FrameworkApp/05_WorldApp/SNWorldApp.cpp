#include "SNWorldApp.h"
#include "SNGraphicsResManager.h"

SNWorldApp::SNWorldApp()
{
	SetSceneNum(3);

	//                   0   1   2   3
	SetScene(&Off,       1, -1, -1, -1);	// 0 OFF
	SetScene(&Creation,  2, -1, -1, -1);	// 1 Creation
	SetScene(&Test,      1, -1, -1, -1);	// 2 Test

	return;
}

SNWorldApp::~SNWorldApp()
{
	return;
}

Void SNWorldApp::OnInitialize()
{

	return;
}


Void SNWorldApp::OnTerminate()
{
	return;
}
