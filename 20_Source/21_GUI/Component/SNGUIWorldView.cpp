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

	ViewScale = SNWViewScaleNormal;

	ViewDir = SNWorldDirN;

	FocusVisible = false;

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

// 初期化
Void SNGUIWorldView::OnInitialize()
{
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNRect rect;
	SNSize size;

	// サーフェス生成
	rect = GetRect();

	size.Width = (Int32)(rect.Width / SNWViewScaleMin);
	size.Height = (Int32)(rect.Height / SNWViewScaleMin);

	grc->CreateBitmap(&WorkSurface, &size);

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
		SNWorld::GetNearbySpace()->RegisterFocus(&TargetPos);
	}

	SortObject();

	DrawWrokSurface();

	return;
}

Void SNGUIWorldView::SortObject()
{
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
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
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNList* list = SNWorld::GetNearbySpace()->GetList();
	Int32 list_num = list->GetNum();
	Int32 cnt;
	SNListContainer* it;
	SNWorldPos scr_offset;
	SNPoint draw_base;
	SNSize size;
	SNColor color = { 0, 0, 0, 0 };

	// 描画の基点となる座標を求める
	// カメラのターゲット座標と、空間座標(左上低)の差分を取る
	scr_offset.X = SNWorld::GetNearbySpace()->GetBasePos()->X - TargetPos.X;
	scr_offset.Y = SNWorld::GetNearbySpace()->GetBasePos()->Y - TargetPos.Y;
	scr_offset.Z = SNWorld::GetNearbySpace()->GetBasePos()->Z - TargetPos.Z;

	// 描画基点を計算
	// 中心ブロックの左上までのずれを補正し
	// 周辺空間左上低までのオフセットを計算/加算する
	WorkSurface.GetSize(&size);
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

	grc->Begin(&WorkSurface);
	grc->Clear(&color);

	it = list->GetTop();

	// リスト内のオブジェクトループ
	for (cnt = 0; cnt < list_num; cnt++)
	{
		// オブジェクト描画
		DrawNearbyObject(grc, (SNWNearbyObject*)it->UserData, &draw_base);

		it = it->Next;
	}

	grc->End();

	return;
}

// 描画
Void SNGUIWorldView::OnDraw(SNGraphicsContext *grc)
{
	SNRect dst_rect;
	SNRect src_rect;
	SNSize size;
	SNBitmap* bg_bmp = SNGraphicsResManager::GetResource(BGRes[SNWorld::GetTimeZone()]);
	SNSize bg_size;
	SNRect bg_rect;

	dst_rect = CalcGlobalRect();
	WorkSurface.GetSize(&size);
	bg_bmp->GetSize(&bg_size);
	bg_rect.PointX = 0;
	bg_rect.PointY = 0;
	bg_rect.Width = bg_size.Width;
	bg_rect.Height = bg_size.Height;
	grc->DrawImage(&dst_rect, bg_bmp, &bg_rect, SNAlphaMax);

	src_rect.Width = (Int32)(dst_rect.Width / ViewScale);
	src_rect.Height = (Int32)(dst_rect.Height / ViewScale);
	src_rect.PointX = (size.Width - src_rect.Width) / 2;
	src_rect.PointY = (size.Height - src_rect.Height) / 2;

	grc->DrawImage(&dst_rect, &WorkSurface, &src_rect, SNAlphaMax);

	grc->DrawImage(&dst_rect, bg_bmp, &bg_rect, TimeZoneAlpha);

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


// 周辺オブジェクト描画
Void SNGUIWorldView::DrawNearbyObject(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
{
	switch (obj->Type)
	{
	case SNWNearbyObjectTypeGround:
		DrawNearbyObjectGround(grc, obj, draw_base);
		break;
	case SNWNearbyObjectTypeEffectGround:
		DrawNearbyObjectEffectGround(grc, obj, draw_base);
		break;
	case SNWNearbyObjectTypeActiveObject:
		DrawNearbyObjectActiveObject(grc, obj, draw_base);
		break;
	case SNWNearbyObjectTypeFocus:
		DrawNearbyObjectFocus(grc, obj, draw_base);
		break;
	}

	return;
}

Void SNGUIWorldView::DrawNearbyObjectGround(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
{
	UInt16 code;
	UInt16 chip_code = (UInt16)(intptr_t)obj->UserData;

	// マップチップ取得
	code = SNMapchip::Data[chip_code].Code[SNWorld::GetAGroundAnimeStep()];

	// 描画
	DrawGround(grc, obj, code, draw_base);

	return;
}

Void SNGUIWorldView::DrawNearbyObjectEffectGround(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
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
		DrawGround(grc, obj, code, draw_base);
	}


	//////////////////////////////////////////////////
	// 境界
	if ((effect_flg & border_mask[ViewDir][0]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirR];
		DrawGround(grc, obj, code, draw_base);
	}
	if ((effect_flg & border_mask[ViewDir][1]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirL];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][2]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirRB];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][3]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirLB];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][4]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderDirT];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][5]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderSideR];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][6]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderSideL];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & border_mask[ViewDir][7]) != 0)
	{
		code = SNMapchip::BorderCode[SNWorldGroundBorderBottom];
		DrawGround(grc, obj, code, draw_base);
	}

	//////////////////////////////////////////////////
	// 投影
	if ((effect_flg & pshadow_mask[ViewDir][0]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowSideR];
		DrawGround(grc, obj, code, draw_base);
	}
	if ((effect_flg & pshadow_mask[ViewDir][1]) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShadowSideL];
		DrawGround(grc, obj, code, draw_base);
	}

	if ((effect_flg & SNWNearbyEffectGroundBitPShadowT) != 0)
	{
		code = SNMapchip::ShadowCode[SNWorldShaodwDirT];
		DrawGround(grc, obj, code, draw_base);
	}

	return;
}


Void SNGUIWorldView::DrawNearbyObjectActiveObject(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
{
	SNWObjectBase* obj_ptr = (SNWObjectBase*)obj->UserData;

	SNWObjectchip::SNWActState act_state = obj_ptr->GetActState();
	SNWorldDir obj_dir = obj_ptr->CalcDir(ViewDir);

	UInt16 code = obj_ptr->GetCode();

	// 描画
	DrawActiveObject(grc, obj, code, obj_dir, act_state, draw_base);

	return;
}


// フォーカス描画
Void SNGUIWorldView::DrawNearbyObjectFocus(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
{
	Int32 x, y, z;

	x = (Int32)obj->Pos.X;
	y = (Int32)obj->Pos.Y;
	z = (Int32)obj->Pos.Z;

	if (SNWorld::GetNearbySpace()->IsBlocked(x, y, z, SNWNearbyObjectTypeGround))
	{
		DrawGround(grc, obj, SNMapchip::SelectedCode, draw_base);
	}

	DrawGround(grc, obj, SNMapchip::FocusCode, draw_base);

	return;
}

Void SNGUIWorldView::DrawGround(SNGraphicsContext* grc, SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNRect src_rect;
	SNRect dst_rect;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// 描画座標計算
	dst_rect.PointX = (Int32)(draw_base->X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * obj->Pos.Z));
	dst_rect.PointY = (Int32)(draw_base->Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * obj->Pos.Z));

	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// マップチップ本体を描画
	grc->DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource[SNMapchip::CodeToResID(code)]),
		&src_rect,
		SNAlphaMax);

	return;
}


Void SNGUIWorldView::DrawGroundBorder(SNGraphicsContext* grc, SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNRect src_rect;
	SNRect dst_rect;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// 描画座標計算
	dst_rect.PointX = (Int32)(draw_base->X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * obj->Pos.Z));
	dst_rect.PointY = (Int32)(draw_base->Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * obj->Pos.Z));

	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// マップチップ本体を描画
	grc->DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource[SNMapchip::CodeToResID(code)]),
		&src_rect,
		SNAlphaMax);

	return;
}

Void SNGUIWorldView::DrawGroundShadow(SNGraphicsContext* grc, SNWNearbyObject* obj, UInt16 code, SNPoint* draw_base)
{
	SNRect src_rect;
	SNRect dst_rect;

	// チップ側の矩形取得
	SNMapchip::CodeToRect(code, ViewDir, &src_rect);

	// 描画座標計算
	dst_rect.PointX = (Int32)(draw_base->X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * obj->Pos.Z));
	dst_rect.PointY = (Int32)(draw_base->Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * obj->Pos.Z));

	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// マップチップ本体を描画
	grc->DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource[SNMapchip::CodeToResID(code)]),
		&src_rect,
		SNAlphaMax);

	return;
}


Void SNGUIWorldView::DrawActiveObject(SNGraphicsContext* grc, SNWNearbyObject* obj, UInt16 code, SNWorldDir obj_dir, SNWObjectchip::SNWActState act_state, SNPoint* draw_base)
{
	SNRect src_rect;
	SNRect dst_rect;

	// チップ側の矩形取得
	SNWObjectchip::CodeToRect(code, obj_dir, act_state, &src_rect);

	// 描画座標計算
	dst_rect.PointX = (Int32)(draw_base->X
		+ (SNMapchip::MapchipStrideX[ViewDir].X * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].X * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].X * obj->Pos.Z))
		+ SNMapchip::MapchipBottomCenterOffset[ViewDir].X
		- SNWObjectchip::WObjectCenterOffset[obj_dir].X;
	dst_rect.PointY = (Int32)(draw_base->Y
		+ (SNMapchip::MapchipStrideX[ViewDir].Y * obj->Pos.X
			+ SNMapchip::MapchipStrideY[ViewDir].Y * obj->Pos.Y
			+ SNMapchip::MapchipStrideZ[ViewDir].Y * obj->Pos.Z))
		+ SNMapchip::MapchipBottomCenterOffset[ViewDir].Y
		- SNWObjectchip::WObjectCenterOffset[obj_dir].Y;

	dst_rect.Width = src_rect.Width;
	dst_rect.Height = src_rect.Height;

	// マップチップ本体を描画
	grc->DrawImage(
		&dst_rect,
		SNGraphicsResManager::GetResource(SNWObjectchip::ObjectchipResource[SNWObjectchip::CodeToResID(code)]),
		&src_rect,
		SNAlphaMax);

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
