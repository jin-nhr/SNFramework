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
	static Boolean bsts = false;

	// 操作説明
	if (pd->SelectPush())
	{
		// 操作説明表示要求
	}

	// 手前ブロック透過設定Down
	if (pd->L1Push() || pd->L1Repeat())
	{
		WorldView.DownTransparentFrontGround();
	}

	// 手前ブロック透過設定Up
	if (pd->R1Push() || pd->R1Repeat())
	{
		WorldView.UpTransparentFrontGround();
	}

	// 拡大
	if (pd->RStkUpPush() || pd->RStkUpRepeat())
	{
		WorldView.UpViewScale();
	}

	// 縮小
	if (pd->RStkDownPush() || pd->RStkDownRepeat())
	{
		WorldView.DownViewScale();
	}

	// 左回転
	if (pd->RStkLeftPush() || pd->RStkLeftRepeat())
	{
		WorldView.RotateLViewDir();
	}

	// 右回転
	if (pd->RStkRightPush() || pd->RStkRightRepeat())
	{
		WorldView.RotateRViewDir();
	}


	// 左上
	if (pd->LStkLeftPress() && pd->LStkUpPress())
	{
		dir = WorldView.UpLeftToAngle();
		dir_input = true;
	}

	// 右上
	else if (pd->LStkRightPress() && pd->LStkUpPress())
	{
		dir = WorldView.UpRightToAngle();
		dir_input = true;
	}

	// 左下
	else if (pd->LStkLeftPress() && pd->LStkDownPress())
	{
		dir = WorldView.DownLeftToAngle();
		dir_input = true;
	}

	// 右下
	else if (pd->LStkRightPress() && pd->LStkDownPress())
	{
		dir = WorldView.DownRightToAngle();
		dir_input = true;
	}

	// 上
	else if (pd->LStkUpPress())
	{
		dir = WorldView.UpToAngle();
		dir_input = true;
	}

	// 下
	else if (pd->LStkDownPress())
	{
		dir = WorldView.DownToAngle();
		dir_input = true;
	}

	// 左
	else if (pd->LStkLeftPress())
	{
		dir = WorldView.LeftToAngle();
		dir_input = true;
	}

	// 右
	else if (pd->LStkRightPress())
	{
		dir = WorldView.RightToAngle();
		dir_input = true;

	}

	if (dir_input)
	{
		// ダッシュ？
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

	// ジャンプ？
	if (pd->BPush())
	{
		bsts = true;
		JumpTimer.Start(100);
		SNWorld::GetPCObject()->Jump();
	}
	else if (pd->BRelease())
	{
		bsts = false;
		SNWorld::GetPCObject()->JumpEnd();
	}
	else if (JumpTimer.IsTimeout())
	{
		bsts = false;
		SNWorld::GetPCObject()->JumpEnd();
	}
	else if (pd->BPress() && bsts)
	{

	}
	else
	{
		bsts = false;
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
	SNWorldPos* pos = &SNWorld::GetPCObject()->RefInfo()->Pos;

	// PC座標をViewにセットする
	WorldView.SetViewPos(pos);
	SNWorld::SetCurrentPos(pos);
	return;
}

Void SNWorldAppTest::OnPreDraw()
{
	SNWorldPos pos;

	WorldView.GetViewPos(&pos);

	txtX.SetValue((Int64)SNMath::FloorToInt(pos.X));
	txtY.SetValue((Int64)SNMath::FloorToInt(pos.Y));
	txtZ.SetValue((Int64)SNMath::FloorToInt(pos.Z));

	SNWorldAppBase::OnPreDraw();
	Win.PreDraw();

	return;
}

Void SNWorldAppTest::OnDraw()
{
	SNWorldDir dir = WorldView.GetViewDir();
	SNWorldShadowDir shadow_dir = SNWorldShadowDirR;

	SNWorldAppBase::OnDraw();

	Win.Draw();

	return;
}

