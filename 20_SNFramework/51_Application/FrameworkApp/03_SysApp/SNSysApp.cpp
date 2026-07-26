#include "SNSysApp.h"


SNSysApp::SNSysApp()
{
	SetSceneNum(5);
	// TransCode        0   1   2   3
	SetScene(&Startup,  1, -1, -1, -1);	// 0
	SetScene(&Loading,  3, -1, -1,  2);	// 1
	SetScene(&Error,   -1, -1, -1, -1);	// 2
	SetScene(&Idle,    -1, -1, -1,  4);	// 3
	SetScene(&PreClose, 3, -1, -1, -1); // 4

	return;
}

SNSysApp::~SNSysApp()
{
	return;
}
