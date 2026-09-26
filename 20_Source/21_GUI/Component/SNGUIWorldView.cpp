#include "SNGUIWorldView.h"
#include "SNGraphicsDevice.h"
#include "SNSystemConfig.h"
#include "SNWorld.h"
#include "SNMath.h"
#include "SNGraphicsResManager.h"
#include "SNWindowsAPI.h"

// コンストラクタ
SNGUIWorldView::SNGUIWorldView()
{
	TargetPos = { 0 };

	RangeStart = TargetPos;

	ViewScale = SNWViewScaleNormal;

	ViewDir = SNWorldDirN;

	FocusVisible = false;

	TransparentFrontGround = SNGUIWorldViewFrontTransparentTypeLow;

	WorkSurfaceSize = {0};

	return;
}

// デストラクタ
SNGUIWorldView::~SNGUIWorldView()
{
	return;
}

// View座標設定
Void SNGUIWorldView::SetViewPos(SNWorldPos* pos)
{
	TargetPos = *pos;
	return;
}

// View座標移動
Void SNGUIWorldView::MoveViewPos(SNWorldPos* pos)
{
	TargetPos.X += pos->X;
	TargetPos.Y += pos->Y;
	TargetPos.Z += pos->Z;
	return;
}

Void SNGUIWorldView::SaveFocusRangeStart()
{
	// 現在位置を範囲開始にセット
	RangeStart = TargetPos;

	return;
}

// スケール設定
Void SNGUIWorldView::SetViewScale(Float32 scale)
{
	ViewScale = scale;

	return;
}

// 拡大
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

// 縮小
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

// View座標取得
Void SNGUIWorldView::GetViewPos(SNWorldPos* pos)
{
	*pos = TargetPos;

	return;
}

Void SNGUIWorldView::GetRangeStartPos(SNWorldPos* pos)
{
	*pos = RangeStart;

	return;
}

// 初期化
Void SNGUIWorldView::OnInitialize()
{
	SNRect rect;
	SNSize size;

	// サーフェス生成
	rect = GetRect();

	size.Width = (Int32)(rect.Width / SNWViewScaleMin);
	size.Height = (Int32)(rect.Height / SNWViewScaleMin);

	SNGraphicsDevice::CreateBitmap(&WorkSurface, &size);

	WorkSurface.GetSize(&WorkSurfaceSize);

	return;
}

Void SNGUIWorldView::OnTerminate()
{
	WorkSurface.DeleteBitmap();

	return;
}

// View方向設定
Void SNGUIWorldView::SetViewDir(SNWorldDir dir)
{
	ViewDir = dir;

	return;
}

// View方向右回転
Void SNGUIWorldView::RotateRViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Increment(ViewDir, SNWorldDirN, SNWorldDirNW);

	return;
}

// View方向左回転
Void SNGUIWorldView::RotateLViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Decrement(ViewDir, SNWorldDirN, SNWorldDirNW);

	return;
}

// View方向取得
SNWorldDir SNGUIWorldView::GetViewDir()
{
	return ViewDir;
}

Void SNGUIWorldView::OnPreDraw()
{
	// 表示設定の場合、フォーカスを登録する
	if (FocusVisible)
	{
		RegisterFocus();
	}

	SortObject();

	DrawWrokSurface();

	return;
}

Void SNGUIWorldView::SortObject()
{
	SNList* list = SNWorld::GetNearbySpace()->GetList();
	SNListCompareFunc func = nullptr;


	switch (ViewDir)
	{
	case SNWorldDirN:
		func = CompareDrawObjectN;
		break;
	case SNWorldDirNE:
		func = CompareDrawObjectNE;
		break;
	case SNWorldDirE:
		func = CompareDrawObjectE;
		break;
	case SNWorldDirSE:
		func = CompareDrawObjectSE;
		break;
	case SNWorldDirS:
		func = CompareDrawObjectS;
		break;
	case SNWorldDirSW:
		func = CompareDrawObjectSW;
		break;
	case SNWorldDirW:
		func = CompareDrawObjectW;
		break;
	case SNWorldDirNW:
		func = CompareDrawObjectNW;
		break;
	default:
		func = CompareDrawObjectN;
		break;
	}

	// ソート実行
	list->Sort(func);

	return;
}

Void SNGUIWorldView::DrawWrokSurface()
{
	SNList* list = SNWorld::GetNearbySpace()->GetList();
	Int32 list_num = list->GetNum();
	Int32 cnt;
	SNListContainer* it;
	SNWorldPos scr_offset;
	SNPoint draw_base;
	SNSize size;

	// 描画の基点となる座標を求める
	// カメラのターゲット座標と、空間座標(左上低)の差分を取る
	scr_offset.X = SNWorld::GetNearbySpace()->GetBasePos()->X - TargetPos.X;
	scr_offset.Y = SNWorld::GetNearbySpace()->GetBasePos()->Y - TargetPos.Y;
	scr_offset.Z = SNWorld::GetNearbySpace()->GetBasePos()->Z - TargetPos.Z;

	// 描画基点を計算
	// 中心ブロックの左上までのずれを補正し
	// 周辺空間左上低までのオフセットを計算/加算する
	size = WorkSurfaceSize;
	draw_base.X = (Int32)(size.Width / 2
		- SNMapchip::MapchipCenterOffset[ViewDir].X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * scr_offset.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * scr_offset.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * scr_offset.Z));

	draw_base.Y = (Int32)(size.Height / 2
		- SNMapchip::MapchipCenterOffset[ViewDir].Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * scr_offset.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * scr_offset.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * scr_offset.Z));

	SNGraphicsDevice::Begin(&WorkSurface);

	it = list->GetTop();

	// リスト内のオブジェクトループ
	for (cnt = 0; cnt < list_num; cnt++)
	{
		// オブジェクト描画
		DrawNearbyObject((SNWNearbyObject*)it->UserData, &draw_base);

		it = it->Next;
	}

	SNGraphicsDevice::End();

	return;
}

// 描画
Void SNGUIWorldView::OnDraw()
{
	SNRect dst_rect;
	SNRect src_rect;
	SNSize size;
	SNBitmap* bg_bmp = SNGraphicsResManager::GetResource(SNMapchip::MapBGResource);
	SNSize bg_size;
	SNRect bg_rect;

	dst_rect = CalcGlobalRect();
	size = WorkSurfaceSize;
	bg_bmp->GetSize(&bg_size);
	bg_rect.PointX = SNMapchip::MapBGBaseX + SNMapchip::MapBGOffsetX * SNWorld::GetTimeZone();
	bg_rect.PointY = SNMapchip::MapBGBaseY + SNMapchip::MapBGOffsetY * SNWorld::GetTimeZone();
	bg_rect.Width = SNMapchip::MapBGWidth;
	bg_rect.Height = SNMapchip::MapBGHeight;
	SNGraphicsDevice::DrawImage(&dst_rect, bg_bmp, &bg_rect, SNAlphaMax);

	src_rect.Width = (Int32)(dst_rect.Width / ViewScale);
	src_rect.Height = (Int32)(dst_rect.Height / ViewScale);
	src_rect.PointX = (size.Width - src_rect.Width) / 2;
	src_rect.PointY = (size.Height - src_rect.Height) / 2;

	SNGraphicsDevice::DrawImage(&dst_rect, &WorkSurface, &src_rect, SNAlphaMax);

	SNGraphicsDevice::DrawImage(&dst_rect, bg_bmp, &bg_rect, TimeZoneAlpha);

	return;
}

// Viewの方向 → ワールドの方位を求める
SNWorldDir SNGUIWorldView::UpToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirN - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::UpLeftToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirNW - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::UpRightToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirNE - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::DownToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirS - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::DownLeftToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirSW - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::DownRightToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirSE - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::LeftToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirW - 1)) % (SNWorldDirNum - 1)) + 1);
}
SNWorldDir SNGUIWorldView::RightToAngle()
{
	return (SNWorldDir)((((SNWorldDirNum - 1) - (ViewDir - 1) + (SNWorldDirE - 1)) % (SNWorldDirNum - 1)) + 1);
}

// View上 → ワールドでの方位
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

// View↓ → ワールドでの方位
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

// Views← → ワールドでの方位
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

// Views→ → ワールドでの方位
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

Void SNGUIWorldView::SetFocusVisible(Boolean visible)
{
	FocusVisible = visible;

	return;
}

Void SNGUIWorldView::UpTransparentFrontGround()
{
	TransparentFrontGround = (SNGUIWorldViewFrontTransparentType)SNMath::Increment(TransparentFrontGround, (Int32)SNGUIWorldViewFrontTransparentTypeOff, (Int32)SNGUIWorldViewFrontTransparentTypeHigh);

	return;
}

Void SNGUIWorldView::DownTransparentFrontGround()
{
	TransparentFrontGround = (SNGUIWorldViewFrontTransparentType)SNMath::Decrement(TransparentFrontGround, (Int32)SNGUIWorldViewFrontTransparentTypeOff, (Int32)SNGUIWorldViewFrontTransparentTypeHigh);

	return;
}

Void SNGUIWorldView::CalcLeftTop(SNWorldPos* pos1, SNWorldPos* pos2, SNWorldPos* lt_pos)
{
	SNWorldPos lefttop;
	SNSize size;
	Int32 z;

	// 座標系における左上点を算出
	if (pos1->X < pos2->X)
	{
		lefttop.X = pos1->X;
	}
	else
	{
		lefttop.X = pos2->X;
	}

	if (pos1->Y < pos2->Y)
	{
		lefttop.Y = pos1->Y;
	}
	else
	{
		lefttop.Y = pos2->Y;
	}

	if (pos1->Z < pos2->Z)
	{
		lefttop.Z = pos1->Z;
	}
	else
	{
		lefttop.Z = pos2->Z;
	}

	size.Width = (Int32)SNMath::AbsF(pos1->X - pos2->X) + 1;
	size.Height = (Int32)SNMath::AbsF(pos1->Y - pos2->Y) + 1;
	z = (Int32)SNMath::AbsF(pos1->Z - pos2->Z) + 1;


	// x, yは方位により補正
	switch (GetViewDir())
	{
	// 左上、右下を選択
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		lt_pos->X = lefttop.X;
		lt_pos->Y = lefttop.Y;
		lt_pos->Z = lefttop.Z;
		break;

	// 右上、左下を選択
	case SNWorldDirE:
	case SNWorldDirSE:
		lt_pos->X = lefttop.X;
		lt_pos->Y = lefttop.Y + size.Height - 1;
		lt_pos->Z = lefttop.Z;
		break;

	// 右下、左上を選択
	case SNWorldDirS:
	case SNWorldDirSW:
		lt_pos->X = lefttop.X + size.Width - 1;
		lt_pos->Y = lefttop.Y + size.Height - 1;
		lt_pos->Z = lefttop.Z;
		break;

	// 左下、右上を選択
	case SNWorldDirW:
	case SNWorldDirNW:
		lt_pos->X = lefttop.X + size.Width - 1;
		lt_pos->Y = lefttop.Y;
		lt_pos->Z = lefttop.Z;
		break;
	}

	return;
}

Void SNGUIWorldView::CalcoRightBottom(SNWorldPos* lt_pos, SNWorldPos* size, SNWorldPos* rb_pos)
{
	// x, yは方位により補正
	switch (GetViewDir())
	{
		// 左上、右下を選択
	case SNWorldDirCenter:
	case SNWorldDirN:
	case SNWorldDirNE:
		rb_pos->X = lt_pos->X + (size->X - 1);
		rb_pos->Y = lt_pos->Y + (size->Y - 1);
		rb_pos->Z = lt_pos->Z + (size->Z - 1);
		break;

		// 右上、左下を選択
	case SNWorldDirE:
	case SNWorldDirSE:
		rb_pos->X = lt_pos->X + (size->X - 1);
		rb_pos->Y = lt_pos->Y - (size->Y - 1);
		rb_pos->Z = lt_pos->Z + (size->Z - 1);
		break;

		// 右下、左上を選択
	case SNWorldDirS:
	case SNWorldDirSW:
		rb_pos->X = lt_pos->X - (size->X - 1);
		rb_pos->Y = lt_pos->Y - (size->Y - 1);
		rb_pos->Z = lt_pos->Z + (size->Z - 1);
		break;

		// 左下、右上を選択
	case SNWorldDirW:
	case SNWorldDirNW:
		rb_pos->X = lt_pos->X - (size->X - 1);
		rb_pos->Y = lt_pos->Y + (size->Y - 1);
		rb_pos->Z = lt_pos->Z + (size->Z - 1);
		break;
	}
	return;
}



// 周辺オブジェクト描画
Void SNGUIWorldView::DrawNearbyObject(SNWNearbyObject* obj, SNPoint* draw_base)
{
	switch (obj->Type)
	{
	case SNWNearbyObjectTypeGround:
		DrawNearbyObjectGround(obj, draw_base);
		break;
	case SNWNearbyObjectTypeEffectGround:
		DrawNearbyObjectEffectGround(obj, draw_base);
		break;
	case SNWNearbyObjectTypeActiveObject:
		DrawNearbyObjectActiveObject(obj, draw_base);
		break;
	case SNWNearbyObjectTypeFocus:
		DrawNearbyObjectFocus(obj, draw_base);
		break;
	}

	return;
}

Void SNGUIWorldView::DrawNearbyObjectGround(SNWNearbyObject* obj, SNPoint* draw_base)
{
	UInt16 code;
	UInt16 chip_code = (UInt16)(intptr_t)obj->UserData;

	// マップチップ取得
	code = SNMapchip::Data[chip_code].Code[SNWorld::GetAGroundAnimeStep()];

	// 描画
	DrawGround(obj, code, draw_base);

	return;
}

Void SNGUIWorldView::DrawNearbyObjectEffectGround(SNWNearbyObject* obj, SNPoint* draw_base)
{
	UInt64 effect_flg = (UInt64)(intptr_t)obj->UserData;
	UInt16 code;

	static constexpr UInt64 glight_mask[SNWorldDirNum][4] =
	{
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL},	// center
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL},	// N - N
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL},	// NE - N
		{SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB},	// E - E
		{SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB},	// SE - E
		{SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR},	// S - S
		{SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR},	// SW - S
		{SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU},	// W - W
		{SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU},	// NW - W
	};

	static constexpr UInt64 border_mask[SNWorldDirNum][8] =
	{
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, 0, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderSideU,SNWNearbyEffectGroundBitBorderSideL,SNWNearbyEffectGroundBitBorderBottom},		// center
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, 0, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderSideR,SNWNearbyEffectGroundBitBorderSideL,SNWNearbyEffectGroundBitBorderBottom},		// N - N
		{SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderB, 0, SNWNearbyEffectGroundBitBorderSideU,SNWNearbyEffectGroundBitBorderSideL,SNWNearbyEffectGroundBitBorderBottom},		// NE - N
		{SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderR, 0, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderSideU,SNWNearbyEffectGroundBitBorderSideB,SNWNearbyEffectGroundBitBorderBottom},		// E - E
		{SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderR, 0, SNWNearbyEffectGroundBitBorderSideL,SNWNearbyEffectGroundBitBorderSideB,SNWNearbyEffectGroundBitBorderBottom},		// SE - E
		{SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderU, 0, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderSideL,SNWNearbyEffectGroundBitBorderSideR,SNWNearbyEffectGroundBitBorderBottom},		// S - S
		{SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderU, 0, SNWNearbyEffectGroundBitBorderSideB,SNWNearbyEffectGroundBitBorderSideR,SNWNearbyEffectGroundBitBorderBottom},		// SW - S
		{SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderL, 0, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderSideB,SNWNearbyEffectGroundBitBorderSideU,SNWNearbyEffectGroundBitBorderBottom},		// W - W
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderL, 0, SNWNearbyEffectGroundBitBorderSideR,SNWNearbyEffectGroundBitBorderSideU,SNWNearbyEffectGroundBitBorderBottom},		// NW - W
	};

	static constexpr UInt64 pshadow_mask[SNWorldDirNum][2] =
	{
		{SNWNearbyEffectGroundBitPShadowB, 0 },	// center
		{SNWNearbyEffectGroundBitPShadowB, 0 },	// N - N
		{SNWNearbyEffectGroundBitPShadowR, SNWNearbyEffectGroundBitPShadowB },	// NE - N
		{SNWNearbyEffectGroundBitPShadowR, 0 },	// E - E
		{SNWNearbyEffectGroundBitPShadowU, SNWNearbyEffectGroundBitPShadowR },	// SE - E
		{SNWNearbyEffectGroundBitPShadowU, 0 },	// S - S
		{SNWNearbyEffectGroundBitPShadowL, SNWNearbyEffectGroundBitPShadowU },	// SW - S
		{SNWNearbyEffectGroundBitPShadowL, 0 },	// W - W
		{SNWNearbyEffectGroundBitPShadowB, SNWNearbyEffectGroundBitPShadowL },	// NW - W
	};


	code = 0;

	//////////////////////////////////////////////////
	// グローバル光源の影描画

	if ((effect_flg & glight_mask[ViewDir][0]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowDirU];
	}
	else if ((effect_flg & glight_mask[ViewDir][1]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowDirR];
	}
	else if ((effect_flg & glight_mask[ViewDir][2]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowDirB];
	}
	else if ((effect_flg & glight_mask[ViewDir][3]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowDirL];
	}

	if (code != 0)
	{
		// 描画
		DrawGround(obj, code, draw_base);
	}


	//////////////////////////////////////////////////
	// 境界
	if ((effect_flg & border_mask[ViewDir][0]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirR];
		DrawGround(obj, code, draw_base);
	}
	if ((effect_flg & border_mask[ViewDir][1]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirL];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][2]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirRB];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][3]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirLB];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][4]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirT];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][5]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderSideR];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][6]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderSideL];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][7]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderBottom];
		DrawGround(obj, code, draw_base);
	}

	//////////////////////////////////////////////////
	// 投影
	if ((effect_flg & pshadow_mask[ViewDir][0]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowSideR];
		DrawGround(obj, code, draw_base);
	}
	if ((effect_flg & pshadow_mask[ViewDir][1]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowSideL];
		DrawGround(obj, code, draw_base);
	}

	if ((effect_flg & SNWNearbyEffectGroundBitPShadowT) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShaodwDirT];
		DrawGround(obj, code, draw_base);
	}

	return;
}


Void SNGUIWorldView::DrawNearbyObjectActiveObject(SNWNearbyObject* obj, SNPoint* draw_base)
{
	SNWObjectBase* obj_ptr = (SNWObjectBase*)obj->UserData;

	SNWObjectchip::SNWActState act_state = obj_ptr->GetActState();
	SNWorldDir obj_dir = obj_ptr->CalcDir(ViewDir);

	UInt16 code = obj_ptr->GetCode();

	// 描画
	DrawActiveObject(obj, code, obj_dir, act_state, draw_base);

	return;
}


// フォーカス描画
Void SNGUIWorldView::DrawNearbyObjectFocus(SNWNearbyObject* obj, SNPoint* draw_base)
{
	Int32 x, y, z;

	x = (Int32)obj->Pos.X;
	y = (Int32)obj->Pos.Y;
	z = (Int32)obj->Pos.Z;

	if (SNWorld::GetNearbySpace()->IsBlocked(x, y, z, SNWNearbyObjectTypeGround))
	{
		DrawGround(obj, SNMapchip::SelectedCode, draw_base);
	}

	DrawGround(obj, SNMapchip::FocusCode, draw_base);

	return;
}

Void SNGUIWorldView::DrawGround(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNPoint pos;
	SNRect src_rect;
	SNRect dst_rect;
	Float32 a_gain = 0.0f;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// チップの描画先座標計算
	CalcMapchipDrawPos(obj, draw_base, &pos);

	dst_rect.PointX = pos.X;
	dst_rect.PointY = pos.Y;
	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// 手前ブロック透過h判定
	a_gain = JudgeFrontTransparent(obj, &dst_rect);

	// マップチップ本体を描画
	SNGraphicsDevice::DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource),
		&src_rect,
		(UInt8)(SNAlphaMax * a_gain));

	return;
}


Void SNGUIWorldView::DrawGroundBorder(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNPoint pos;
	SNRect src_rect;
	SNRect dst_rect;
	Float32 a_gain = 0.0f;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// チップの描画先座標計算
	CalcMapchipDrawPos(obj, draw_base, &pos);

	dst_rect.PointX = pos.X;
	dst_rect.PointY = pos.Y;
	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// 手前ブロック透過h判定
	a_gain = JudgeFrontTransparent(obj, &dst_rect);

	// マップチップ本体を描画
	SNGraphicsDevice::DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource),
		&src_rect,
		(UInt8)(SNAlphaMax * a_gain));

	return;
}

Void SNGUIWorldView::DrawGroundShadow(SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNPoint pos;
	SNRect src_rect;
	SNRect dst_rect;
	Float32 a_gain = 0.0f;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// チップの描画先座標計算
	CalcMapchipDrawPos(obj, draw_base, &pos);

	dst_rect.PointX = pos.X;
	dst_rect.PointY = pos.Y;
	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// 手前ブロック透過h判定
	a_gain = JudgeFrontTransparent(obj, &dst_rect);

	// マップチップ本体を描画
	SNGraphicsDevice::DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource),
		&src_rect,
		(UInt8)(SNAlphaMax * a_gain));

	return;
}


Void SNGUIWorldView::DrawActiveObject(SNWNearbyObject* obj, UInt16 code, SNWorldDir obj_dir, SNWObjectchip::SNWActState act_state, SNPoint* draw_base)
{
	SNPoint pos;
	SNRect src_rect;
	SNRect dst_rect;
	Float32 a_gain = 0.0f;

	// チップ側の矩形取得
	SNWObjectchip::CodeToRect(code, obj_dir, act_state, &src_rect);

	// チップの描画先座標計算
	CalcMapchipDrawPos(obj, draw_base, &pos);

	dst_rect.PointX = pos.X 
		+ SNMapchip::MapchipBottomCenterOffset[ViewDir].X
		- SNWObjectchip::WObjectCenterOffset[obj_dir].X;
	dst_rect.PointY = pos.Y
		+ SNMapchip::MapchipBottomCenterOffset[ViewDir].Y
		- SNWObjectchip::WObjectCenterOffset[obj_dir].Y;
	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// 手前ブロック透過h判定
	a_gain = JudgeFrontTransparent(obj, &dst_rect);

	// マップチップ本体を描画
	SNGraphicsDevice::DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNWObjectchip::ObjectchipResource),
		&src_rect,
		(UInt8)(SNAlphaMax * a_gain));

	return;
}


Void SNGUIWorldView::CalcMapchipDrawPos(SNWNearbyObject* obj, SNPoint* draw_base, SNPoint* out)
{
	// 描画座標計算
	out->X = (Int32)(draw_base->X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * obj->Pos.Z));
	out->Y = (Int32)(draw_base->Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * obj->Pos.Z));
	return;
}

Float32 SNGUIWorldView::JudgeFrontTransparent(SNWNearbyObject* obj, SNRect* dst_rect)
{
	Float32 ret = TransGainOff;
	Int32 dx = 0;
	Int32 dy = 0;
	Int32 dist2 = 0;

	dx = dst_rect->PointX - (WorkSurfaceSize.Width / 2);
	dy = dst_rect->PointY - (WorkSurfaceSize.Height / 2);

	dist2 = dx * dx + dy * dy;

	// 高さが一定以上、かつ一定範囲内の場合は透過率を変更
	if ((obj->Pos.Z > SNSystemConfig::WorldNearbySpaceSizeV + TransRangeMinZ) &&
		(dist2 < TransViewRange * TransViewRange))
	{
		switch (TransparentFrontGround)
		{
		case SNGUIWorldViewFrontTransparentTypeOff:
			ret = TransGainOff;
			break;
		case SNGUIWorldViewFrontTransparentTypeLow:
			ret = TransGainLow;
			break;
		case SNGUIWorldViewFrontTransparentTypeHigh:
			ret = TransGainHigh;
			break;
		}
	}

	return ret;
}


Void SNGUIWorldView::RegisterFocus()
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

	GetRangeStartPos(&range_st);
	GetViewPos(&range_ed);

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

				SNWorld::GetNearbySpace()->RegisterFocus(&write_pos);
			}
		}
	}

	return;
}


// ソート用比較関数
// true:aを選択、false:bを選択
Boolean SNGUIWorldView::CompareDrawObjectN(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if (ap->Y < bp->Y)	ret = true;
	else if (ap->Y > bp->Y) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectNE(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if ((ap->X + ap->Y) < (bp->X + bp->Y)) ret = true;
	else if ((ap->X + ap->Y) > (bp->X + bp->Y)) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectE(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if (ap->X < bp->X)	ret = true;
	else if (ap->X > bp->X) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectSE(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if ((ap->X - ap->Y) < (bp->X - bp->Y)) ret = true;
	else if ((ap->X - ap->Y) > (bp->X - bp->Y)) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectS(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if (ap->Y > bp->Y)	ret = true;
	else if (ap->Y < bp->Y) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectSW(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if ((- ap->X - ap->Y) < (- bp->X - bp->Y)) ret = true;
	else if ((- ap->X - ap->Y) > (- bp->X - bp->Y)) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectW(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if (ap->X > bp->X)	ret = true;
	else if (ap->X < bp->X) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

Boolean SNGUIWorldView::CompareDrawObjectNW(Void* a, Void* b)
{
	Boolean ret = false;
	SNWorldPos* ap = &((SNWNearbyObject*)a)->Pos;
	SNWorldPos* bp = &((SNWNearbyObject*)b)->Pos;
	SNWNearbyObjectType at = ((SNWNearbyObject*)a)->Type;
	SNWNearbyObjectType bt = ((SNWNearbyObject*)b)->Type;

	if ((- ap->X + ap->Y) < (- bp->X + bp->Y)) ret = true;
	else if ((- ap->X + ap->Y) > (- bp->X + bp->Y)) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}

