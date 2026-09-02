#include "SNWorldAppTest.h"
#include "SNMusicPlayer.h"
#include "SNVirtualGamePad.h"
#include "SNWorld.h"
#include "SNGraphicsResManager.h"
#include "SNMath.h"


SNWorldAppTest::SNWorldAppTest()
{
	SelectBlock = 0;

	return;
}

SNWorldAppTest::~SNWorldAppTest()
{
	return;
}

Void SNWorldAppTest::OnInitialize()
{
	SNWorldAppBase::OnInitialize();
	
	Win.SetSceneNum(3);
	Win.SetScene(&txtX);
	Win.SetScene(&txtY);
	Win.SetScene(&txtZ);

	Win.Initialize();

	WinBlock.Initialize();

	SelectBlock = SNMapchip::SNMapchipGreen;

	return;
}

Void SNWorldAppTest::OnTerminate()
{
	SNWorldAppBase::OnTerminate();

	Win.Terminate();

	WinBlock.Terminate();

	return;
}

Void SNWorldAppTest::OnEntry()
{
	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResEnterWorld);

	SNWorldAppBase::OnEntry();

	WorldView.SetFocusVisible(false);

	Win.Entry();

	Win.SetRect(784, 420, 160, 104);
	txtX.SetRect(16, 16, 128, 24);
	txtY.SetRect(16, 40, 128, 24);
	txtZ.SetRect(16, 64, 128, 24);

	txtX.SetText((String)L"X=%+06d");
	txtY.SetText((String)L"Y=%+06d");
	txtZ.SetText((String)L"Z=%+06d");

	WinBlock.Entry();

	WinBlock.SetRect(16, 460, 64, 64);
	WinBlock.Centering(false, true);

	SNWorld::GetPCObject()->SetEnable(true);
	SNWorld::GetPCObject()->SetVisible(true);

	return;
}

Void SNWorldAppTest::OnExit()
{
	Win.Exit();

	SNWorldAppBase::OnExit();

	SNWorld::GetPCObject()->SetEnable(false);
	SNWorld::GetPCObject()->SetVisible(false);

	return;
}

Boolean SNWorldAppTest::OnGamePad1()
{
	Boolean ret = true;

	return ret;
}

Boolean SNWorldAppTest::OnInternalEvent()
{
	Boolean ret = false;

	if (SNEvent::InternalEvent[SNEventResultMenuMapCreate])
	{
		TransCode = SNTransitionCode0;
		ret = true;
	}

	return ret;
}

Void SNWorldAppTest::OnPreDraw()
{
	SNWorldPos pos;

	SNWorldAppBase::OnPreDraw();

	WorldView.GetViewPos(&pos);

	txtX.SetValue((Int64)pos.X);
	txtY.SetValue((Int64)pos.Y);
	txtZ.SetValue((Int64)pos.Z);

	WorldView.PreDraw();
	Win.PreDraw();

	return;
}

Void SNWorldAppTest::OnDraw(SNGraphicsContext* grc)
{
	SNBitmap* bmp = SNGraphicsResManager::GetResource(SNGraphicsResMapchip1);
	SNRect win_rect;
	SNRect dst_rect;
	SNRect src_rect;
	SNWorldDir dir = WorldView.GetViewDir();
	SNWorldShadowDir shadow_dir = SNWorldShadowDirR;

	SNWorldAppBase::OnDraw(grc);

	Win.Draw(grc);
	WinBlock.Draw(grc);

	// ウインドウに選択中のマップチップを描画する
	win_rect = WinBlock.CalcGlobalRect();

	SNMapchip::CodeToRect(SNMapchip::Data[SelectBlock].Code[0], dir, &src_rect);

	dst_rect.PointX = win_rect.PointX + (win_rect.Width - (src_rect.Width * 2)) / 2;
	dst_rect.PointY = win_rect.PointY + (win_rect.Height - (src_rect.Height * 2)) / 2;
	dst_rect.Width = src_rect.Width * 2;
	dst_rect.Height = src_rect.Height * 2;

	grc->DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);
	grc->DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	return;
}

