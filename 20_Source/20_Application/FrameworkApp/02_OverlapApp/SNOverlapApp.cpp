#include "SNOverlapApp.h"


SNOverlapApp::SNOverlapApp()
{
	SetSceneNum(2);
	SetScene(&MuteApp);
	SetScene(&Focus);

	return;
}

SNOverlapApp::~SNOverlapApp()
{
	return;
}
