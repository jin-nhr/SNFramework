#include "SNSysApp.h"


SNSysApp::SNSysApp()
{
	SetSceneNum(1);
	SetScene(&Startup, -1, -1, -1, -1);

	return;
}

SNSysApp::~SNSysApp()
{
	return;
}
