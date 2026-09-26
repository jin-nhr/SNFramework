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

	CopySpaceSize = { 0 };
	ZeroMemory(CopySpace, sizeof(CopySpace));

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
	WorldView.SaveFocusRangeStart();

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

	// 手前ブロック透過設定
	if (pd->L1Push() || pd->L1Repeat())
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

	// ペースト
	if (pd->R2Push())
	{
		Paste();
		// コマンド実行
		SNWorld::FlushGroundData();

		RangeMode = false;
		WorldView.SaveFocusRangeStart();
	}

	// ブロック削除
	if (pd->R1Push() || pd->R1Repeat())
	{
		WriteBlock(SNMapchip::SNMapchipBlank);

		SNWorld::FlushGroundData();
	}

	// ブロック配置
	if (pd->APush() || pd->ARepeat())
	{
		WriteBlock(SelectBlock);

		SNWorld::FlushGroundData();
	}

	// 範囲選択モード切替
	if (pd->BPush())
	{
		if (RangeMode)
		{
			// 範囲選択モード中はメニュー表示
			SNEvent::EventResult[SNEventResultDspCreationMenu] = true;
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

	if (SNEvent::InternalEvent[SNEventResultMenuCopy])
	{
		RangeCopy();
		RangeMode = false;
		WorldView.SaveFocusRangeStart();
		ret = true;
	}

	if (SNEvent::InternalEvent[SNEventResultMenuCut])
	{
		RangeCut();
		RangeMode = false;
		WorldView.SaveFocusRangeStart();
		ret = true;
	}

	if (SNEvent::InternalEvent[SNEventResultMenuRotateL])
	{
		RotateL();
		ret = true;
	}

	if (SNEvent::InternalEvent[SNEventResultMenuRotateR])
	{
		RotateR();
		ret = true;
	}

	if (SNEvent::InternalEvent[SNEventResultMenuDeselect])
	{
		RangeMode = false;
		WorldView.SaveFocusRangeStart();
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


Void SNWorldAppCreation::WriteBlock(UInt16 code)
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

				SNWorld::WriteGroundData(&write_pos, (SNMapchip::SNMapchipCode)code);
			}
		}
	}

	return;
}

Void SNWorldAppCreation::RangeCopy()
{
	SNWNearbySpace* space = SNWorld::GetNearbySpace();
	SNWNearbyObject* obj = nullptr;
	SNWorldPos st;
	SNWorldPos ed;
	SNWorldPos* nearby_o;

	Int32 st_x;
	Int32 st_y;
	Int32 st_z;
	Int32 num_x;
	Int32 num_y;
	Int32 num_z;
	Int32 cnt_x;
	Int32 cnt_y;
	Int32 cnt_z;

	// 選択範囲取得
	WorldView.GetRangeStartPos(&st);
	WorldView.GetViewPos(&ed);
	nearby_o = space->GetBasePos();

	// 開始座標、ブロック数を計算
	if (st.X <= ed.X)
	{
		st_x = SNMath::FloorToInt(st.X);
		num_x = SNMath::FloorToInt(ed.X) - st_x + 1;
	}
	else
	{
		st_x = SNMath::FloorToInt(ed.X);
		num_x = SNMath::FloorToInt(st.X) - st_x + 1;
	}

	if (st.Y <= ed.Y)
	{
		st_y = SNMath::FloorToInt(st.Y);
		num_y = SNMath::FloorToInt(ed.Y) - st_y + 1;
	}
	else
	{
		st_y = SNMath::FloorToInt(ed.Y);
		num_y = SNMath::FloorToInt(st.Y) - st_y + 1;
	}

	if (st.Z <= ed.Z)
	{
		st_z = SNMath::FloorToInt(st.Z);
		num_z = SNMath::FloorToInt(ed.Z) - st_z + 1;
	}
	else
	{
		st_z = SNMath::FloorToInt(ed.Z);
		num_z = SNMath::FloorToInt(st.Z) - st_z + 1;
	}

	// 周辺オブジェクト用座標に変換
	st_x -= SNMath::FloorToInt(nearby_o->X);
	st_y -= SNMath::FloorToInt(nearby_o->Y);
	st_z -= SNMath::FloorToInt(nearby_o->Z);


	// Z, Y, X軸ループし構造をバッファに保存する
	for (cnt_z = 0; cnt_z < num_z; cnt_z++)
	{
		for (cnt_y = 0; cnt_y < num_y; cnt_y++)
		{
			for (cnt_x = 0; cnt_x < num_x; cnt_x++)
			{
				obj = space->RefObjectG(st_x + cnt_x, st_y + cnt_y, st_z + cnt_z);
				if (obj != nullptr)
				{
					CopySpace[cnt_z][cnt_y][cnt_x] = (UInt16)(intptr_t)obj->UserData;
				}
				else
				{
					CopySpace[cnt_z][cnt_y][cnt_x] = (UInt16)SNMapchip::SNMapchipBlank;
				}
			}
		}
	}

	// サイズをセット
	CopySpaceSize.X = (Float32)num_x;
	CopySpaceSize.Y = (Float32)num_y;
	CopySpaceSize.Z = (Float32)num_z;

	return;
}

Void SNWorldAppCreation::RangeCut()
{
	// コピー
	RangeCopy();

	// 削除
	WriteBlock(SNMapchip::SNMapchipBlank);

	SNWorld::FlushGroundData();

	return;
}

Void SNWorldAppCreation::Paste()
{
	SNWNearbySpace* space = SNWorld::GetNearbySpace();
	SNWorldPos st;
	SNWorldPos set_pos;
	SNWorldDir view_dir = WorldView.GetViewDir();

	Int32 st_x;
	Int32 st_y;
	Int32 st_z;
	Int32 num_x = SNMath::FloorToInt(CopySpaceSize.X);
	Int32 num_y = SNMath::FloorToInt(CopySpaceSize.Y);
	Int32 num_z = SNMath::FloorToInt(CopySpaceSize.Z);
	Int32 cnt_x;
	Int32 cnt_y;
	Int32 cnt_z;

	// フォーカス位置取得
	WorldView.GetViewPos(&st);

	st_x = SNMath::FloorToInt(st.X);
	st_y = SNMath::FloorToInt(st.Y);
	st_z = SNMath::FloorToInt(st.Z);
	

	// Z, Y, X軸ループし、Blank以外をワールドに書き込む
	for (cnt_z = 0; cnt_z < num_z; cnt_z++)
	{
		for (cnt_y = 0; cnt_y < num_y; cnt_y++)
		{
			for (cnt_x = 0; cnt_x < num_x; cnt_x++)
			{
				if (CopySpace[cnt_z][cnt_y][cnt_x] != SNMapchip::SNMapchipBlank)
				{
					set_pos.Z = (Float32)(st_z + cnt_z);

					switch (view_dir)
					{
					case SNWorldDirCenter:
					case SNWorldDirN:
					case SNWorldDirNE:
						set_pos.Y = (Float32)(st_y + cnt_y);
						set_pos.X = (Float32)(st_x + cnt_x);
						break;

					case SNWorldDirE:
					case SNWorldDirSE:
						set_pos.Y = (Float32)(st_y + cnt_y - (num_y - 1));
						set_pos.X = (Float32)(st_x + cnt_x);
						break;

					case SNWorldDirS:
					case SNWorldDirSW:
						set_pos.Y = (Float32)(st_y + cnt_y - (num_y - 1));
						set_pos.X = (Float32)(st_x + cnt_x - (num_x - 1));
						break;

					case SNWorldDirW:
					case SNWorldDirNW:
						set_pos.Y = (Float32)(st_y + cnt_y);
						set_pos.X = (Float32)(st_x + cnt_x - (num_x - 1));
						break;
					}

					SNWorld::WriteGroundData(&set_pos, (SNMapchip::SNMapchipCode)CopySpace[cnt_z][cnt_y][cnt_x]);
				}
			}
		}
	}

	return;
}

Void SNWorldAppCreation::RotateL()
{
	SNWNearbySpace* space = SNWorld::GetNearbySpace();
	SNWNearbyObject* obj = nullptr;
	SNWorldPos st;
	SNWorldPos ed;
	SNWorldPos* nearby_o;

	SNWorldPos lt_pos;
	SNWorldPos rb_pos;

	Int32 st_x;
	Int32 st_y;
	Int32 st_z;
	Int32 num_x;
	Int32 num_y;
	Int32 num_z;
	Int32 cnt_x;
	Int32 cnt_y;
	Int32 cnt_z;

	// 選択範囲取得
	WorldView.GetRangeStartPos(&st);
	WorldView.GetViewPos(&ed);
	nearby_o = space->GetBasePos();

	// 開始座標、ブロック数を計算
	if (st.X <= ed.X)
	{
		st_x = SNMath::FloorToInt(st.X);
		num_x = SNMath::FloorToInt(ed.X) - st_x + 1;
	}
	else
	{
		st_x = SNMath::FloorToInt(ed.X);
		num_x = SNMath::FloorToInt(st.X) - st_x + 1;
	}

	if (st.Y <= ed.Y)
	{
		st_y = SNMath::FloorToInt(st.Y);
		num_y = SNMath::FloorToInt(ed.Y) - st_y + 1;
	}
	else
	{
		st_y = SNMath::FloorToInt(ed.Y);
		num_y = SNMath::FloorToInt(st.Y) - st_y + 1;
	}

	if (st.Z <= ed.Z)
	{
		st_z = SNMath::FloorToInt(st.Z);
		num_z = SNMath::FloorToInt(ed.Z) - st_z + 1;
	}
	else
	{
		st_z = SNMath::FloorToInt(ed.Z);
		num_z = SNMath::FloorToInt(st.Z) - st_z + 1;
	}

	// 周辺オブジェクト用座標に変換
	st_x -= SNMath::FloorToInt(nearby_o->X);
	st_y -= SNMath::FloorToInt(nearby_o->Y);
	st_z -= SNMath::FloorToInt(nearby_o->Z);


	// Z, Y, X軸ループし構造をバッファに保存する
	for (cnt_z = 0; cnt_z < num_z; cnt_z++)
	{
		for (cnt_y = 0; cnt_y < num_y; cnt_y++)
		{
			for (cnt_x = 0; cnt_x < num_x; cnt_x++)
			{
				obj = space->RefObjectG(st_x + cnt_x, st_y + cnt_y, st_z + cnt_z);
				if (obj != nullptr)
				{
					CopySpace[cnt_z][num_x - cnt_x - 1][cnt_y] = (UInt16)(intptr_t)obj->UserData;
				}
				else
				{
					CopySpace[cnt_z][num_x - cnt_x - 1][cnt_y] = (UInt16)SNMapchip::SNMapchipBlank;
				}
			}
		}
	}

	// サイズをセット (回転によりxとyが入れ替わる)
	CopySpaceSize.X = (Float32)num_y;
	CopySpaceSize.Y = (Float32)num_x;
	CopySpaceSize.Z = (Float32)num_z;

	// 削除
	WriteBlock(SNMapchip::SNMapchipBlank);

	// 回転にあわせてフォーカス補正
	WorldView.CalcLeftTop(&st, &ed, &lt_pos);
	WorldView.CalcoRightBottom(&lt_pos, &CopySpaceSize, &rb_pos);

	WorldView.SetViewPos(&rb_pos);
	WorldView.SaveFocusRangeStart();
	WorldView.SetViewPos(&lt_pos);
	SNWorld::SetCurrentPos(&lt_pos);

	// 貼り付け
	Paste();

	// コマンド実行
	SNWorld::FlushGroundData();

	return;
}

Void SNWorldAppCreation::RotateR()
{
	SNWNearbySpace* space = SNWorld::GetNearbySpace();
	SNWNearbyObject* obj = nullptr;
	SNWorldPos st;
	SNWorldPos ed;
	SNWorldPos* nearby_o;

	SNWorldPos lt_pos;
	SNWorldPos rb_pos;

	Int32 st_x;
	Int32 st_y;
	Int32 st_z;
	Int32 num_x;
	Int32 num_y;
	Int32 num_z;
	Int32 cnt_x;
	Int32 cnt_y;
	Int32 cnt_z;

	// 選択範囲取得
	WorldView.GetRangeStartPos(&st);
	WorldView.GetViewPos(&ed);
	nearby_o = space->GetBasePos();

	// 開始座標、ブロック数を計算
	if (st.X <= ed.X)
	{
		st_x = SNMath::FloorToInt(st.X);
		num_x = SNMath::FloorToInt(ed.X) - st_x + 1;
	}
	else
	{
		st_x = SNMath::FloorToInt(ed.X);
		num_x = SNMath::FloorToInt(st.X) - st_x + 1;
	}

	if (st.Y <= ed.Y)
	{
		st_y = SNMath::FloorToInt(st.Y);
		num_y = SNMath::FloorToInt(ed.Y) - st_y + 1;
	}
	else
	{
		st_y = SNMath::FloorToInt(ed.Y);
		num_y = SNMath::FloorToInt(st.Y) - st_y + 1;
	}

	if (st.Z <= ed.Z)
	{
		st_z = SNMath::FloorToInt(st.Z);
		num_z = SNMath::FloorToInt(ed.Z) - st_z + 1;
	}
	else
	{
		st_z = SNMath::FloorToInt(ed.Z);
		num_z = SNMath::FloorToInt(st.Z) - st_z + 1;
	}

	// 周辺オブジェクト用座標に変換
	st_x -= SNMath::FloorToInt(nearby_o->X);
	st_y -= SNMath::FloorToInt(nearby_o->Y);
	st_z -= SNMath::FloorToInt(nearby_o->Z);


	// Z, Y, X軸ループし構造をバッファに保存する
	for (cnt_z = 0; cnt_z < num_z; cnt_z++)
	{
		for (cnt_y = 0; cnt_y < num_y; cnt_y++)
		{
			for (cnt_x = 0; cnt_x < num_x; cnt_x++)
			{
				obj = space->RefObjectG(st_x + cnt_x, st_y + cnt_y, st_z + cnt_z);
				if (obj != nullptr)
				{
					CopySpace[cnt_z][cnt_x][num_y - cnt_y - 1] = (UInt16)(intptr_t)obj->UserData;
				}
				else
				{
					CopySpace[cnt_z][cnt_x][num_y - cnt_y - 1] = (UInt16)SNMapchip::SNMapchipBlank;
				}
			}
		}
	}

	// サイズをセット (回転によりxとyが入れ替わる)
	CopySpaceSize.X = (Float32)num_y;
	CopySpaceSize.Y = (Float32)num_x;
	CopySpaceSize.Z = (Float32)num_z;

	// 削除
	WriteBlock(SNMapchip::SNMapchipBlank);

	// 回転にあわせてフォーカス補正
	WorldView.CalcLeftTop(&st, &ed, &lt_pos);
	WorldView.CalcoRightBottom(&lt_pos, &CopySpaceSize, &rb_pos);

	WorldView.SetViewPos(&rb_pos);
	WorldView.SaveFocusRangeStart();
	WorldView.SetViewPos(&lt_pos);
	SNWorld::SetCurrentPos(&lt_pos);

	// 貼り付け
	Paste();

	// コマンド実行
	SNWorld::FlushGroundData();

	return;
}
