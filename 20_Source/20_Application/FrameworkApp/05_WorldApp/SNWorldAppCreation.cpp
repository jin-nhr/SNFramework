#include "SNWorldAppCreation.h"
#include "SNMusicPlayer.h"
#include "SNVirtualGamePad.h"
#include "SNWorld.h"
#include "SNGraphicsResManager.h"
#include "SNMath.h"
#include "SNFocus.h"
#include "SNGraphicsDevice.h"
#include "SNInput.h"
#include "SNVGamePadNStyle.h"
#include "SNWindowsAPI.h"

SNWorldAppCreation::SNWorldAppCreation()
{
	SelectBlock = 0;

	return;
}

SNWorldAppCreation::~SNWorldAppCreation()
{
	return;
}

Void SNWorldAppCreation::OnInitialize()
{
	SNWorldAppBase::OnInitialize();
	
	Win.SetSceneNum(3);
	Win.SetScene(&txtX);
	Win.SetScene(&txtY);
	Win.SetScene(&txtZ);

	Win.Initialize();

	WinBlock.Initialize();

	SelectBlock = SNMapchip::SNMapchipBlank;

	RangeMode = false;

	CopiedFlag = false;

	ZeroMemory(CopySpace, sizeof(CopySpace));
	ZeroMemory(WorkSpace, sizeof(WorkSpace));

	return;
}

Void SNWorldAppCreation::OnTerminate()
{
	SNWorldAppBase::OnTerminate();

	Win.Terminate();

	WinBlock.Terminate();

	return;
}

Void SNWorldAppCreation::OnEntry()
{
	SNMusicPlayer::Play(SNMusicLayerLowPri, SNMusicResEnterWorld);

	SNWorldAppBase::OnEntry();

	WorldView.SetFocusVisible(true);

	Win.Entry();

	Win.SetRect(784, 420, 160, 104);
	txtX.SetRect(16, 16, 128, 24);
	txtY.SetRect(16, 40, 128, 24);
	txtZ.SetRect(16, 64, 128, 24);

	txtX.SetText((String)L"X=%+06d");
	txtY.SetText((String)L"Y=%+06d");
	txtZ.SetText((String)L"Z=%+06d");

	WinBlock.Entry();

	WinBlock.SetRect(16, 460, 320, 64);
	WinBlock.Centering(false, true);

	SNWorld::GetPCObject()->SetEnable(false);
	SNWorld::GetPCObject()->SetVisible(false);

	RangeMode = false;

	return;
}

Void SNWorldAppCreation::OnExit()
{
	Win.Exit();

	SNWorldAppBase::OnExit();

	return;
}

Boolean SNWorldAppCreation::OnGamePad1()
{
	Boolean ret = true;
	SNWorldPos current = { 0.0f, 0.0f, 0.0f };

	SNVGamePadNStyle* pd = SNInput::RefN1();

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

	// Z軸Up
	if (pd->DPadUpPush() || pd->DPadUpRepeat())
	{
		current.Z = 1.0f;
	}

	// Z軸Down
	if (pd->DPadDownPush() || pd->DPadDownRepeat())
	{
		current.Z = -1.0f;
	}

	// ブロック切替Left
	if (pd->DPadLeftPush() || pd->DPadLeftRepeat())
	{
		SelectBlock = (UInt16)SNMath::Decrement(SelectBlock, SNMapchip::SNMapchipBlank, SNMapchip::SNMapchipNum - 1);
	}

	// ブロック切替Right
	if (pd->DPadRightPush() || pd->DPadRightRepeat())
	{
		SelectBlock = (UInt16)SNMath::Increment(SelectBlock, SNMapchip::SNMapchipBlank, SNMapchip::SNMapchipNum - 1);

	}

	// フォーカスUp
	if (pd->LStkUpPush() || pd->LStkUpRepeat())
	{
		WorldView.UpToWorldDirPos(&current.X, &current.Y);
	}

	// フォーカスDown
	if (pd->LStkDownPush() || pd->LStkDownRepeat())
	{
		WorldView.DownToWorldDirPos(&current.X, &current.Y);
	}

	// フォーカスLeft
	if (pd->LStkLeftPush() || pd->LStkLeftRepeat())
	{
		WorldView.LeftToWorldDirPos(&current.X, &current.Y);
	}

	// フォーカスRight
	if (pd->LStkRightPush() || pd->LStkRightRepeat())
	{
		WorldView.RightToWorldDirPos(&current.X, &current.Y);
	}

	if (pd->YPress())
	{
		current.X = current.X * FocusAcc;
		current.Y = current.Y * FocusAcc;
	}

	WorldView.MoveViewPos(&current);
	SNWorld::MoveCurrentPos(&current);
	if (!RangeMode)
	{
		// 範囲選択モードではないときは常に範囲開始位置を更新
		WorldView.SaveFocusRangeStart();
	}


	// ブロック配置
	if (pd->APush() || pd->ARepeat())
	{
		WriteBlock();
	}

	// 範囲選択モード切替
	if (pd->BPush())
	{
		if (RangeMode)
		{
			// 範囲選択モード中はメニュー表示
			RangeMode = false;	// 仮で範囲モード解除★
		}

		else
		{
			// 範囲選択モードへ移行する
			RangeMode = true;
		}
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

	return ret;
}

Boolean SNWorldAppCreation::OnInternalEvent()
{
	Boolean ret = false;

	if (SNEvent::InternalEvent[SNEventResultMenuTestPlay])
	{
		TransCode = SNTransitionCode0;
		ret = true;
	}

	return ret;
}

Void SNWorldAppCreation::OnPreDraw()
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

Void SNWorldAppCreation::OnDraw()
{
	SNBitmap* bmp = SNGraphicsResManager::GetResource(SNGraphicsResMapchip1);
	SNRect win_rect;
	SNRect dst_rect;
	SNRect dst_tmp;
	UInt16 code_tmp;
	SNRect src_rect;
	Int32 cnt;
	SNWorldDir dir = WorldView.GetViewDir();
	SNWorldShadowDir shadow_dir = SNWorldShadowDirR;

	SNWorldAppBase::OnDraw();

	Win.Draw();
	WinBlock.Draw();

	// ウインドウに選択中のマップチップを描画する
	win_rect = WinBlock.CalcGlobalRect();

	SNMapchip::CodeToRect(SNMapchip::Data[SelectBlock].Code[0], dir, &src_rect);

	dst_rect.PointX = win_rect.PointX + (win_rect.Width - (src_rect.Width * 2)) / 2;
	dst_rect.PointY = win_rect.PointY + (win_rect.Height - (src_rect.Height * 2)) / 2;
	dst_rect.Width = src_rect.Width * 2;
	dst_rect.Height = src_rect.Height * 2;

	// 選択中ブロック描画
	SNGraphicsDevice::DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	// 選択中ブロックに影を付加
	SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);
	SNGraphicsDevice::DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);

	// フォーカス描画
	SNMapchip::CodeToRect(SNMapchip::FocusCode, dir, &src_rect);
	SNGraphicsDevice::DrawImage(&dst_rect, bmp, &src_rect, SNAlphaMax);


	dst_tmp = dst_rect;
	code_tmp = SelectBlock;

	// リスト左表示
	for (cnt = 0; cnt < MapchipListView; cnt++)
	{
		code_tmp = (UInt16)SNMath::Decrement(code_tmp, SNMapchip::SNMapchipBlank, SNMapchip::SNMapchipNum - 1);

		SNMapchip::CodeToRect(SNMapchip::Data[code_tmp].Code[0], dir, &src_rect);

		dst_tmp.PointX -= dst_tmp.Width;

		// 選択中ブロック描画
		SNGraphicsDevice::DrawImage(&dst_tmp, bmp, &src_rect, SNAlphaMax);

		// 選択中ブロックに影を付加
		SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);
		SNGraphicsDevice::DrawImage(&dst_tmp, bmp, &src_rect, SNAlphaMax);
	}

	dst_tmp = dst_rect;
	code_tmp = SelectBlock;

	// リスト右表示
	for (cnt = 0; cnt < MapchipListView; cnt++)
	{
		code_tmp = (UInt16)SNMath::Increment(code_tmp, SNMapchip::SNMapchipBlank, SNMapchip::SNMapchipNum - 1);

		SNMapchip::CodeToRect(SNMapchip::Data[code_tmp].Code[0], dir, &src_rect);

		dst_tmp.PointX += dst_tmp.Width;

		// 選択中ブロック描画
		SNGraphicsDevice::DrawImage(&dst_tmp, bmp, &src_rect, SNAlphaMax);

		// 選択中ブロックに影を付加
		SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);
		SNGraphicsDevice::DrawImage(&dst_tmp, bmp, &src_rect, SNAlphaMax);
	}

	return;
}


Void SNWorldAppCreation::WriteBlock()
{
	SNWorldPos range_st;
	SNWorldPos range_ed;
	SNWorldPos write_pos = { 0.0f };
	Int32 from_x;
	Int32 from_y;
	Int32 from_z;
	Int32 to_x;
	Int32 to_y;
	Int32 to_z;
	Int32 step_x = 1;
	Int32 step_y = 1;
	Int32 step_z = 1;
	Int32 pos_x = 0;
	Int32 pos_y = 0;
	Int32 pos_z = 0;
	
	WorldView.GetRangeStartPos(&range_st);
	WorldView.GetViewPos(&range_ed);

	from_x = SNMath::FloorToInt(range_st.X);
	from_y = SNMath::FloorToInt(range_st.Y);
	from_z = SNMath::FloorToInt(range_st.Z);

	to_x = SNMath::FloorToInt(range_ed.X);
	to_y = SNMath::FloorToInt(range_ed.Y);
	to_z = SNMath::FloorToInt(range_ed.Z);

	if (to_x < from_x)
	{
		step_x = -1;
	}
	if (to_y < from_y)
	{
		step_y = -1;
	}
	if (to_z < from_z)
	{
		step_z = -1;
	}

	// 範囲ループ
	for (pos_z = from_z; pos_z != (to_z + step_z); pos_z += step_z)
	{
		write_pos.Z = (Float32)pos_z;

		for (pos_y = from_y; pos_y != (to_y + step_y); pos_y += step_y)
		{
			write_pos.Y = (Float32)pos_y;

			for (pos_x = from_x; pos_x != (to_x + step_x); pos_x += step_x)
			{
				write_pos.X = (Float32)pos_x;

				SNWorld::WriteGroundData(&write_pos, (SNMapchip::SNMapchipCode)SelectBlock);
			}
		}
	}

	SNWorld::FlushGroundData();

	return;
}
