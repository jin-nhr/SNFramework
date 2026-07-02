#include "TestApplication.h"


#if 0

TestApplication UserApp;


TestApplication::TestApplication()
{
	return;
}


TestApplication::~TestApplication()
{
	return;
}


Void TestApplication::Initialize()
{
	fader.SetColor(SNColorBlack);
	fader.SetAlpha(SNAlphaMax);
	fader.SetFadeType(SNGUIFadeTypeCenterToSide);

	text.Initialize();
	text.SetText((String)L"テキスト表示のテスト");
	text.Move(100, 100);
	text.SetTextColor(SNColorYellow);
	text.SetShadowEnable(true);
	text.SetShadowColor(SNColorRed);
	text.SetShadowOffset(2);

	bg.SetFolderFileName((String)L"bg");
	bg.SetAsyncMode(false);
	bg.Load();

	rect.Initialize();
	rect.SetRect(0, 0, 960, 540);
	rect.SetColor(SNColorRed);
	rect.SetAlpha(100);

	circle.Initialize();
	circle.SetRect(500, 440, 100, 100);
	circle.SetColor(SNColorBlue);

	SNPoint pnt[2] = { 200, 200, 500, 300 };

	line.Initialize();
	line.SetColor(SNColorGreen);
	line.SetPoint(pnt, 2);

	image.Initialize();
	image.SetRect(100, 100, 250, 250);
	image.SetSurface(&bg);

	return;
}

Void TestApplication::Terminate()
{
	text.Terminate();
	fader.Terminate();
	rect.Terminate();
	circle.Terminate();
	line.Terminate();
	image.Terminate();
	return;
}


Void TestApplication::Entry()
{
	text.Entry();
	fader.Entry();
	fader.FadeIn(3000);
	rect.Entry();
	circle.Entry();
	line.Entry();
	image.Entry();
	return;
}

Void TestApplication::Exit()
{
	text.Exit();
	fader.Exit();
	rect.Exit();
	circle.Exit();
	line.Exit();
	image.Exit();
	return;
}

SNTransitionCode TestApplication::Step(SNEvent* event)
{
	text.Step(event);
	fader.Step(event);
	rect.Step(event);
	circle.Step(event);
	line.Step(event);
	image.Step(event);
	return SNTransitionCodeStay;
}

Void TestApplication::Draw(SNSurface* surface)
{
	surface->StretchBlt(0, 0, 960, 540, bg.GetDC(), 0, 0, 960, 540);
	text.Draw(surface);
	fader.Draw(surface);
	rect.Draw(surface);
	circle.Draw(surface);
	line.Draw(surface);
	image.Draw(surface);
	return;
}

#endif
