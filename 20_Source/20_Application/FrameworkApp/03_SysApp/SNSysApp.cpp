#include "SNSysApp.h"


SNSysApp::SNSysApp()
{
	SetSceneNum(6);
	// TransCode        0   1   2   3
	SetScene(&Startup,  1, -1, -1, -1);	// 0
	SetScene(&Loading,  3, -1, -1,  2);	// 1
	SetScene(&Error,   -1, -1, -1, -1);	// 2
	SetScene(&Idle,     5, -1, -1,  4);	// 3	 b’è‚Å5‚Ö‚Ì‘JˆÚ‚ğİ’è
	SetScene(&PreClose, 3, -1, -1, -1); // 4
	SetScene(&Config,   3, -1, -1,  4);	// 5

	return;
}

SNSysApp::~SNSysApp()
{
	return;
}
