#include "SNUserApp.h"
#include "SNGraphicsResManager.h"

SNUserApp::SNUserApp()
{
	SetSceneNum(1);
	SetScene(&Picture);
	return;
}

SNUserApp::~SNUserApp()
{
	return;
}

Void SNUserApp::OnPreDraw()
{
	Picture.SetBitmap(SNGraphicsResManager::GetResource(SNGraphicsResAppDummy));

	return;
}
