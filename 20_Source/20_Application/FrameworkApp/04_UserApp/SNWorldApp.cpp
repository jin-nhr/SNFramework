#include "SNWorldApp.h"
#include "SNMusicPlayer.h"
#include "SNVirtualGamePad.h"
#include "SNWorld.h"
#include "SNGraphicsResManager.h"
#include "SNMath.h"

SNWorldApp::SNWorldApp()
{
	return;
}

SNWorldApp::~SNWorldApp()
{
	return;
}

Void SNWorldApp::OnInitialize()
{
	WorldView.Initialize();
	
	Win.SetSceneNum(3);
	Win.SetScene(&txtX);
	Win.SetScene(&txtY);
	Win.SetScene(&txtZ);

	Win.Initialize();

	WinBlock.Initialize();

	SelectBlock = SNMapchip::SNMapchipGreen;

	return;
}

Void SNWorldApp::OnEntry()
{
	SNWorldPos current = { 0.0f, 0.0f, 0.0f };

	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResEnterWorld);
	SNWorld::Start();

	SNWorld::SetCurrentPos(&current);
	WorldView.Entry();
	WorldView.SetViewPos(&current);

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

	return;
}

Void SNWorldApp::OnExit()
{
	Win.Exit();
	WorldView.Exit();

	SNMusicPlayer::Stop(SNMusicLayerLowPri);
	SNWorld::End();

	return;
}

Boolean SNWorldApp::OnGamePad1()
{
	Boolean ret = true;
	SNWorldPos current = { 0.0f, 0.0f, 0.0f };

	// あげる
	if (((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventPush]) ||
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventRepeat])) &&
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadAction][SNVirtualGamePadEventPress]))
	{
		current.Z = 1.0f;
	}

	// 上
	else if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadUp][SNVirtualGamePadEventRepeat]))
	{
		WorldView.UpToWorldDirPos(&current.X, &current.Y);
	}

	// さげる
	if (((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventPush]) ||
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventRepeat])) &&
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadAction][SNVirtualGamePadEventPress]))
	{
		current.Z = -1.0f;
	}

	// 下
	else if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDown][SNVirtualGamePadEventRepeat]))
	{
		WorldView.DownToWorldDirPos(&current.X, &current.Y);
	}

	// Action+左 ブロック選択
	if (((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventPush]) ||
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat])) &&
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadAction][SNVirtualGamePadEventPress]))
	{
		SelectBlock = (UInt16)SNMath::Decrement(SelectBlock, SNMapchip::SNMapchipBlank + 1, SNMapchip::SNMapchipNum - 1);
	}

	// 左
	else if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadLeft][SNVirtualGamePadEventRepeat]))
	{
		WorldView.LeftToWorldDirPos(&current.X, &current.Y);
	}

	// Action+右 ブロック選択
	if (((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventPush]) ||
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventRepeat])) &&
		 (SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadAction][SNVirtualGamePadEventPress]))
	{
		SelectBlock = (UInt16)SNMath::Increment(SelectBlock, SNMapchip::SNMapchipBlank + 1, SNMapchip::SNMapchipNum - 1);
	}

	// 右
	else if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadRight][SNVirtualGamePadEventRepeat]))
	{
		WorldView.RightToWorldDirPos(&current.X, &current.Y);
	}

	WorldView.MoveViewPos(&current);
	SNWorld::MoveCurrentPos(&current);


	// 拡大
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadListUp][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadListUp][SNVirtualGamePadEventRepeat]))
	{
		WorldView.UpViewScale();
	}

	// 縮小
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadListDown][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadListDown][SNVirtualGamePadEventRepeat]))
	{
		WorldView.DownViewScale();
	}

	// 右回転
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPageNext][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPageNext][SNVirtualGamePadEventRepeat]))
	{
		WorldView.RotateRViewDir();
	}

	// 左回転
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPagePrev][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPagePrev][SNVirtualGamePadEventRepeat]))
	{
		WorldView.RotateLViewDir();
	}


	// 置く
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDecide][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadDecide][SNVirtualGamePadEventRepeat]))
	{
		SNWorld::WriteGroundData((SNMapchip::SNMapchipCode)SelectBlock);
	}

	// 消す
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadCancel][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadCancel][SNVirtualGamePadEventRepeat]))
	{
		SNWorld::WriteGroundData(SNMapchip::SNMapchipBlank);
	}

	return ret;
}

Void SNWorldApp::OnPreDraw()
{
	SNWorldPos pos;

	WorldView.GetViewPos(&pos);

	txtX.SetValue((Int64)pos.X);
	txtY.SetValue((Int64)pos.Y);
	txtZ.SetValue((Int64)pos.Z);

	WorldView.PreDraw();
	Win.PreDraw();

	return;
}

Void SNWorldApp::OnDraw(SNGraphicsContext* grc)
{
	SNBitmap* bmp = SNGraphicsResManager::GetResource(SNGraphicsResMapchip1);
	SNRect win_rect;
	SNRect dst_rect;
	SNRect src_rect;
	SNWorldDir dir = WorldView.GetViewDir();
	SNWorldShadowDir shadow_dir = SNWorld::CalcShadowDir(dir);

	WorldView.Draw(grc);
	Win.Draw(grc);
	WinBlock.Draw(grc);

	// ウインドウに選択中のマップチップを描画する
	win_rect = WinBlock.CalcGlobalRect();

	SNMapchip::CodeToRect(SNMapchip::Data[SelectBlock].Code, dir, &src_rect);

	dst_rect.PointX = win_rect.PointX + (win_rect.Width - (src_rect.Width * 2)) / 2;
	dst_rect.PointY = win_rect.PointY + (win_rect.Height - (src_rect.Height * 2)) / 2;
	dst_rect.Width = src_rect.Width * 2;
	dst_rect.Height = src_rect.Height * 2;

	grc->DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);
	grc->DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	return;
}

