#include "SNWorldAppBase.h"
#include "SNWorld.h"
#include "SNGraphicsResManager.h"
#include "SNMath.h"

SNWorldAppBase::SNWorldAppBase()
{
	return;
}

SNWorldAppBase::~SNWorldAppBase()
{
	return;
}

Void SNWorldAppBase::OnInitialize()
{
	WorldView.Initialize();
	
	return;
}

Void SNWorldAppBase::OnTerminate()
{
	WorldView.Terminate();

	return;
}

Void SNWorldAppBase::OnEntry()
{
	SNWorldPos current = { 0.0f, 0.0f, 0.0f };
	SNWorld::Start();
	SNWorld::SetCurrentPos(&current);
	WorldView.Entry();
	WorldView.SetViewPos(&current);

	return;
}

Void SNWorldAppBase::OnExit()
{
	WorldView.Exit();
	SNWorld::End();

	return;
}

Boolean SNWorldAppBase::OnGamePad1()
{
	Boolean ret = false;

	return ret;
}

Boolean SNWorldAppBase::OnInternalEvent()
{
	Boolean ret = false;

	return ret;
}

Void SNWorldAppBase::OnPreDraw()
{
	WorldView.PreDraw();

	return;
}

Void SNWorldAppBase::OnDraw(SNGraphicsContext* grc)
{
	WorldView.Draw(grc);

	return;
}

