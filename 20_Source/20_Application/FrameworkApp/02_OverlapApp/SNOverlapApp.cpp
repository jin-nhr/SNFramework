#include "SNOverlapApp.h"


SNOverlapApp::SNOverlapApp()
{
	SetSceneNum(3);
	SetScene(&MuteApp);
	SetScene(&Focus);
	SetScene(&MusicPlayer);

	return;
}

SNOverlapApp::~SNOverlapApp()
{
	return;
}
