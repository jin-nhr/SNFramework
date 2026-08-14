#include "SNUserApp.h"
#include "SNGraphicsResManager.h"

SNUserApp::SNUserApp()
{
	return;
}

SNUserApp::~SNUserApp()
{
	return;
}

Void SNUserApp::OnInitialize()
{
	SetSceneNum(3);

	//                  0   1   2   3
	SetScene(&Off,      1, -1, -1, -1);		// 0 OFF
	SetScene(&Title,    2, -1, -1, -1);		// 1 Title
	SetScene(&Idle,    -1, -1, -1, -1);		// 2 Idle

	return;
}


Void SNUserApp::OnTerminate()
{
	return;
}