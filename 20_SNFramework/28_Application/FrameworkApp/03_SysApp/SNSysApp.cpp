#include "SNSysApp.h"


SNSysApp::SNSysApp()
{
	SetSceneNum(2);
	// TransCode        0   1   2   3
	SetScene(&Startup,  1, -1, -1, -1);
	SetScene(&Loading, -1, -1, -1, -1);

	return;
}

SNSysApp::~SNSysApp()
{
	return;
}
