#include "SNGUIWorldView.h"
#include "SNGraphicsDevice.h"
#include "SNSystemConfig.h"
#include "SNWorld.h"
#include "SNMath.h"

// コンストラクタ
SNGUIWorldView::SNGUIWorldView()
{
	TargetPos = { 0 };

	ViewScale = SNWViewScaleNormal;

	ViewDir = SNWorldDirN;

	return;
}

// デストラクタ
SNGUIWorldView::~SNGUIWorldView()
{
	return;
}

Void SNGUIWorldView::SetViewPos(SNWorldPos* pos)
{
	TargetPos = *pos;
	return;
}

Void SNGUIWorldView::MoveViewPos(SNWorldPos* pos)
{
	TargetPos.X += pos->X;
	TargetPos.Y += pos->Y;
	TargetPos.Z += pos->Z;
	return;
}

Void SNGUIWorldView::SetViewScale(Float32 scale)
{
	ViewScale = scale;

	return;
}

Void SNGUIWorldView::UpViewScale()
{
	if (ViewScale >= SNWViewScaleNormal)
	{
		ViewScale += SNWViewScaleStepHigh;
	}
	else
	{
		ViewScale += SNWViewScaleStepLow;
	}

	ViewScale = SNMath::SaturateF(ViewScale, SNWViewScaleMin, SNWViewScaleMax);

	return;
}

Void SNGUIWorldView::DownViewScale()
{
	if (ViewScale > SNWViewScaleNormal)
	{
		ViewScale -= SNWViewScaleStepHigh;
	}
	else
	{
		ViewScale -= SNWViewScaleStepLow;
	}

	ViewScale = SNMath::SaturateF(ViewScale, SNWViewScaleMin, SNWViewScaleMax);

	return;
}

Void SNGUIWorldView::GetViewPos(SNWorldPos* pos)
{
	*pos = TargetPos;

	return;
}

Void SNGUIWorldView::OnInitialize()
{
	return;
}

Void SNGUIWorldView::SetViewDir(SNWorldDir dir)
{
	ViewDir = dir;

	return;
}

Void SNGUIWorldView::RotateRViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Increment(ViewDir, SNWorldDirN, SNWorldDirNW);

	return;
}

Void SNGUIWorldView::RotateLViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Decrement(ViewDir, SNWorldDirN, SNWorldDirNW);

	return;
}

SNWorldDir SNGUIWorldView::GetViewDir()
{
	return ViewDir;
}


Void SNGUIWorldView::OnDraw(SNGraphicsContext *grc)
{


	return;
}

Void SNGUIWorldView::UpToWorldDirPos(Float32* x, Float32* y)
{
	switch (ViewDir)
	{
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		*y -= 1;
		break;
	case SNWorldDirE:
	case SNWorldDirSE:
		*x -= 1;
		break;
	case SNWorldDirS:
	case SNWorldDirSW:
		*y += 1;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		*x += 1;
		break;
	}
	return;
}

Void SNGUIWorldView::DownToWorldDirPos(Float32* x, Float32* y)
{
	switch (ViewDir)
	{
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		*y += 1;
		break;
	case SNWorldDirE:
	case SNWorldDirSE:
		*x += 1;
		break;
	case SNWorldDirS:
	case SNWorldDirSW:
		*y -= 1;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		*x -= 1;
		break;
	}
	return;
}

Void SNGUIWorldView::LeftToWorldDirPos(Float32* x, Float32* y)
{
	switch (ViewDir)
	{
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		*x -= 1;
		break;
	case SNWorldDirE:
	case SNWorldDirSE:
		*y += 1;
		break;
	case SNWorldDirS:
	case SNWorldDirSW:
		*x += 1;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		*y -= 1;
		break;
	}
	return;
}

Void SNGUIWorldView::RightToWorldDirPos(Float32* x, Float32* y)
{
	switch (ViewDir)
	{
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		*x += 1;
		break;
	case SNWorldDirE:
	case SNWorldDirSE:
		*y -= 1;
		break;
	case SNWorldDirS:
	case SNWorldDirSW:
		*x -= 1;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		*y += 1;
		break;
	}
	return;
}





#if 0
// 地形描画
Void SNWGround::Draw(SNBitmap* target, SNWorldPos* target_pos, Boolean focus, SNWorldDir dir, SNWorldShadowDir shadow_dir, Int32 in_range, Int32 in_range_z)
{
	Int32 x_start;
	Int32 x_num;
	Int32 x_step;

	Int32 y_start;
	Int32 y_num;
	Int32 y_step;

	Int32 z_start;
	Int32 z_num;
	Int32 z_step;

	Int32 x_cnt;
	Int32 y_cnt;
	Int32 z_cnt;

	SNWorldDir out_dir;
	SNWorldElevation z;
	SNWorldPos lpos;

	SNWorldPos in_pos;

	UInt16 code;
	SNRect dst_rect;
	SNRect src_rect;

	SNMapchip::SNMapchipCode chip_code;

	SNSize dst_size;
	SNPoint lt_pos = { 0 };

	Boolean br_l;
	Boolean br_r;
	Boolean br_top;


	switch (dir)
	{
	case SNWorldDirN:
	case SNWorldDirNE:
	case SNWorldDirE:
		x_start = (Int32)(target_pos->X - in_range);
		x_step = 1;
		y_start = (Int32)(target_pos->Y - in_range);
		y_step = 1;
		break;
	case SNWorldDirSE:
	case SNWorldDirS:
		x_start = (Int32)(target_pos->X - in_range);
		x_step = 1;
		y_start = (Int32)(target_pos->Y + in_range);
		y_step = -1;
		break;
	case SNWorldDirSW:
		x_start = (Int32)(target_pos->X + in_range);
		x_step = -1;
		y_start = (Int32)(target_pos->Y + in_range);
		y_step = -1;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		x_start = (Int32)(target_pos->X + in_range);
		x_step = -1;
		y_start = (Int32)(target_pos->Y - in_range);
		y_step = 1;
		break;

	}

	z_start = (Int32)(target_pos->Z - in_range_z);
	z_step = 1;

	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNColor color = { 0, 0, 0, 0 };
	SNBitmap* source = SNGraphicsResManager::GetResource(SNGraphicsResMapchip1);

	// targetの中心座標から地形データ左上角の座標を算出
	target->GetSize(&dst_size);
	lt_pos.X =
		dst_size.Width / 2
		- SNMapchip::MapchipCenterOffset[dir].X
		- (SNMapchip::MapchipStrideX[dir].X * in_range
			+ SNMapchip::MapchipStrideY[dir].X * in_range
			+ SNMapchip::MapchipStrideZ[dir].X * in_range_z);
	lt_pos.Y =
		dst_size.Height / 2
		- SNMapchip::MapchipCenterOffset[dir].Y
		- (SNMapchip::MapchipStrideX[dir].Y * in_range
			+ SNMapchip::MapchipStrideY[dir].Y * in_range
			+ SNMapchip::MapchipStrideZ[dir].Y * in_range_z);

	// ワークに対してテキスト描画
	grc->Begin(target);
	grc->Clear(&color);

	x_num = in_range * 2 + 1;
	y_num = x_num;
	z_num = in_range_z * 2 + 1;

	in_pos.X = (Float32)x_start;
	in_pos.Y = (Float32)y_start;
	in_pos.Z = (Float32)z_start;
	z_cnt = 0;

	while (z_cnt < z_num)
	{
		in_pos.Y = (Float32)y_start;
		y_cnt = 0;

		while (y_cnt < y_num)
		{
			in_pos.X = (Float32)x_start;
			x_cnt = 0;

			while (x_cnt < x_num)
			{
				// 方位等と座標取得
				if (CvtIDAndLocalPos(&in_pos, &out_dir, &z, &lpos))
				{
					chip_code = MeshRef[z][out_dir]->Mesh.GetCode((Int32)lpos.X, (Int32)lpos.Y, (Int32)lpos.Z);

					// マップチップ取得
					code = SNMapchip::Data[chip_code].Code;

					// チップ側の矩形取得
					SNMapchip::CodeToRect(code, dir, &src_rect);

					dst_rect.PointX = lt_pos.X
						+ (SNMapchip::MapchipStrideX[dir].X * x_cnt
							+ SNMapchip::MapchipStrideY[dir].X * y_cnt
							+ SNMapchip::MapchipStrideZ[dir].X * z_cnt);
					dst_rect.PointY = lt_pos.Y
						+ (SNMapchip::MapchipStrideX[dir].Y * x_cnt
							+ SNMapchip::MapchipStrideY[dir].Y * y_cnt
							+ SNMapchip::MapchipStrideZ[dir].Y * z_cnt);

					dst_rect.Width = src_rect.Width;
					dst_rect.Height = src_rect.Height;

					if (chip_code != SNMapchip::SNMapchipBlank)
					{
						// マップチップ本体を描画
						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);

						// 通常の影描画
						SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);

						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);

						// 遮蔽物による影描画判定
						if (judgeDrawShadow(&in_pos, dir, shadow_dir))
						{
							// 全体影
							SNMapchip::CodeToRect(SNMapchip::ShadowCode[SNWorldShaodwDirT], dir, &src_rect);

							grc->DrawImage(
								&dst_rect,
								source,
								&src_rect,
								SNAlphaMax);
						}

						// 境界線の描画判定
						if (JudgeDrawBorder(&in_pos, dir, &br_l, &br_r, &br_top))
						{

						}
					}

					// フォーカス
					if (focus && (x_cnt == y_cnt) && (x_cnt == in_range) && (z_cnt == in_range_z))
					{
						// 影描画
						SNMapchip::CodeToRect(SNMapchip::FocusCode, dir, &src_rect);

						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);
					}
				}
				in_pos.X += x_step;
				x_cnt++;
			}
			in_pos.Y += y_step;
			y_cnt++;
		}
		in_pos.Z += z_step;
		z_cnt++;
	}

	grc->End();

	return;
}


// 影描画判定
Boolean SNWGround::judgeDrawShadow(SNWorldPos* in_pos, SNWorldDir dir, SNWorldShadowDir shadow_dir)
{
	Boolean ret = false;
	SNWorldDir mesh_pos;
	SNWorldPos pos;
	SNWorldPos out_pos;
	SNWorldElevation z;

	// 直上がBlankか
	CvtIDAndLocalPos(in_pos, &mesh_pos, &z, &out_pos);

	{

		// 光源方向(影の反対方向)のブロック検索

		{


		}



	}

	return ret;
}

// 境界描画判定
Boolean SNWGround::JudgeDrawBorder(SNWorldPos* in_pos, SNWorldDir dir, Boolean* br_l, Boolean* br_r, Boolean* br_top)
{
	Boolean ret = false;


	return ret;
}

#endif


