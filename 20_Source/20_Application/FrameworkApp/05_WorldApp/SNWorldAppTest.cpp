#include "SNWorldAppTest.h"
#include "SNMusicPlayer.h"
#include "SNVirtualGamePad.h"
#include "SNWorld.h"
#include "SNGraphicsResManager.h"
#include "SNMath.h"
#include "SNInput.h"


SNWorldAppTest::SNWorldAppTest()
{
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

	return;
}

Void SNWorldAppTest::OnTerminate()
{
	SNWorldAppBase::OnTerminate();

	Win.Terminate();

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
	SNVGamePadNStyle* pd = SNInput::RefN1();
	SNWorldDir dir;
	Boolean dir_input = false;

	// 左上
	if (pd->DPadLeftPress() && pd->DPadUpPress())
	{
		dir = WorldView.UpLeftToAngle();
		dir_input = true;
	}

	// 右上
	else if (pd->DPadRightPress() && pd->DPadUpPress())
	{
		dir = WorldView.UpRightToAngle();
		dir_input = true;
	}

	// 左下
	else if (pd->DPadLeftPress() && pd->DPadDownPress())
	{
		dir = WorldView.DownLeftToAngle();
		dir_input = true;
	}

	// 右下
	else if (pd->DPadRightPress() && pd->DPadDownPress())
	{
		dir = WorldView.DownRightToAngle();
		dir_input = true;
	}

	// 上
	else if (pd->DPadUpPress())
	{
		dir = WorldView.UpToAngle();
		dir_input = true;
	}

	// 下
	else if (pd->DPadDownPress())
	{
		dir = WorldView.DownToAngle();
		dir_input = true;
	}

	// 左
	else if (pd->DPadLeftPress())
	{
		dir = WorldView.LeftToAngle();
		dir_input = true;
	}

	// 右
	else if (pd->DPadRightPress())
	{
		dir = WorldView.RightToAngle();
		dir_input = true;

	}

	if (dir_input)
	{
		if (pd->YPress())
		{
			SNWorld::GetPCObject()->Jog(dir, SNWorld::DirToAngle(dir));
		}
		else
		{
			SNWorld::GetPCObject()->Walk(dir, SNWorld::DirToAngle(dir));
		}
	}

	// 方向入力なし
	else
	{
		SNWorld::GetPCObject()->Stop();
	}



	// 表示変更操作

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
		WorldView.RotateLViewDir();
	}

	// 左回転
	if ((SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPagePrev][SNVirtualGamePadEventPush]) ||
		(SNVirtualGamePad::Event[SNVirtualGamePadID1][SNVirtualGamePadPagePrev][SNVirtualGamePadEventRepeat]))
	{
		WorldView.RotateRViewDir();
	}

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

Void SNWorldAppTest::OnCycle()
{
	// PC座標をViewにセットする
	WorldView.SetViewPos(&SNWorld::GetPCObject()->RefInfo()->Pos);

	return;
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
	SNWorldDir dir = WorldView.GetViewDir();
	SNWorldShadowDir shadow_dir = SNWorldShadowDirR;

	SNWorldAppBase::OnDraw(grc);

	Win.Draw(grc);

	return;
}

