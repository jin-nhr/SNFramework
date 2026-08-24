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
	SNWorld::GetNearbySpace()->RegisterFocus(&TargetPos);

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

	dst_rect = CalcGlobalRect();
	WorkSurface.GetSize(&size);

	src_rect.Width = (Int32)(dst_rect.Width / ViewScale);
	src_rect.Height = (Int32)(dst_rect.Height / ViewScale);
	src_rect.PointX = (size.Width - src_rect.Width) / 2;
	src_rect.PointY = (size.Height - src_rect.Height) / 2;

	grc->DrawImage(&dst_rect, &WorkSurface, &src_rect, SNAlphaMax);

	return;
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
	code = SNMapchip::Data[chip_code].Code;

	// 描画
	DrawGround(grc, obj, code, draw_base);

	return;
}

Void SNGUIWorldView::DrawNearbyObjectEffectGround(SNGraphicsContext* grc, SNWNearbyObject* obj, SNPoint* draw_base)
{
#if 0
	// SNWNearbyObjectTypeEffectGround
// - UInt64 Effect関連Bit
	enum SNWNearbyEffectGroundBit
	{
		// グローバル光源による影
		SNWNearbyEffectGroundBitGShadowU = 0x00000001,
		SNWNearbyEffectGroundBitGShadowR = 0x00000002,
		SNWNearbyEffectGroundBitGShadowB = 0x00000004,
		SNWNearbyEffectGroundBitGShadowL = 0x00000008,

		// 他地形の投影
		SNWNearbyEffectGroundBitPShadowU = 0x00000010,
		SNWNearbyEffectGroundBitPShadowR = 0x00000020,
		SNWNearbyEffectGroundBitPShadowB = 0x00000040,
		SNWNearbyEffectGroundBitPShadowL = 0x00000080,
		SNWNearbyEffectGroundBitPShadowT = 0x00000100,

		// 段差境界
		SNWNearbyEffectGroundBitBorderU = 0x000001000,
		SNWNearbyEffectGroundBitBorderR = 0x000002000,
		SNWNearbyEffectGroundBitBorderB = 0x000004000,
		SNWNearbyEffectGroundBitBorderL = 0x000008000,

	};

	// 影ブロック
	static constexpr UInt16 ShadowCode[SNWorldShadowDirNum] =
	{
		0x0000,
		MapchipBlockNumY + 0x0000,
		MapchipBlockNumY + 0x0001,
		MapchipBlockNumY + 0x0002,
		MapchipBlockNumY + 0x0003,
		MapchipBlockNumY + 0x0004,
		MapchipBlockNumY + 0x0005,
		MapchipBlockNumY + 0x0006,

	};

	// 境界　ブロック
	static constexpr UInt16 BorderCode[SNWorldGroundBorderDirNum] =



		// 影方向
		enum SNWorldShadowDir
	{
		SNWorldShadowNon,
		SNWorldShadowDirR,
		SNWorldShadowDirB,
		SNWorldShadowDirL,
		SNWorldShadowDirU,
		SNWorldShaodwDirT,
		SNWorldShadowSideR,
		SNWorldShadowSideL,
		SNWorldShadowDirNum
	};

	// 境界線
	enum SNWorldGroundBorderDir
	{
		SNWorldGroundBorderNon,
		SNWorldGroundBorderDirR,
		SNWorldGroundBorderDirL,
		SNWorldGroundBorderDirT,
		SNWorldGroundBorderDirNum,
	};

	SNWorldDirCenter,
		SNWorldDirN,
		SNWorldDirNE,
		SNWorldDirE,
		SNWorldDirSE,
		SNWorldDirS,
		SNWorldDirSW,
		SNWorldDirW,
		SNWorldDirNW,

#endif
	UInt64 effect_flg = (UInt64)(intptr_t)obj->UserData;
	UInt16 code;



	static constexpr UInt64 glight_mask[SNWorldDirNum][4] =
	{
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL},	// center
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL},	// N - N
		{SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowL},	// NE - N
		{SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB},	// E - E
		{SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB},	// SE - E
		{SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR},	// S - S
		{SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU, SNWNearbyEffectGroundBitGShadowR},	// SW - S
		{SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU},	// W - W
		{SNWNearbyEffectGroundBitGShadowR, SNWNearbyEffectGroundBitGShadowB, SNWNearbyEffectGroundBitGShadowL, SNWNearbyEffectGroundBitGShadowU},	// NW - W
	};

	static constexpr UInt64 border_mask[SNWorldDirNum][5] =
	{
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, 0, SNWNearbyEffectGroundBitBorderU},		// center
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, 0, SNWNearbyEffectGroundBitBorderU},		// N - N
		{SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderB, 0},		// NE - N
		{SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderR, 0, SNWNearbyEffectGroundBitBorderL},		// E - E
		{SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderR, 0},		// SE - E
		{SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderU, 0, SNWNearbyEffectGroundBitBorderB},		// S - S
		{SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderL, SNWNearbyEffectGroundBitBorderU, 0},		// SW - S
		{SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderL, 0, SNWNearbyEffectGroundBitBorderR},		// W - W
		{SNWNearbyEffectGroundBitBorderR, SNWNearbyEffectGroundBitBorderU, SNWNearbyEffectGroundBitBorderB, SNWNearbyEffectGroundBitBorderL, 0},		// NW - W
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
		SNGraphicsResManager::GetResource(SNMapchip::MapchipResource),
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
	else if (ap->X != bp->X) ret = true;
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

	if (ap->Y < bp->Y)	ret = true;
	else if (ap->Y > bp->Y) ret = false;
	else if (ap->X < bp->X) ret = true;
	else if (ap->X > bp->X) ret = false;
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
	else if (ap->Y != bp->Y) ret = true;
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

	if (ap->Y > bp->Y)	ret = true;
	else if (ap->Y < bp->Y) ret = false;
	else if (ap->X < bp->X) ret = true;
	else if (ap->X > bp->X) ret = false;
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
	else if (ap->X != bp->X) ret = true;
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

	if (ap->Y > bp->Y)	ret = true;
	else if (ap->Y < bp->Y) ret = false;
	else if (ap->X > bp->X) ret = true;
	else if (ap->X < bp->X) ret = false;
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
	else if (ap->Y != bp->Y) ret = true;
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

	if (ap->X > bp->X)	ret = true;
	else if (ap->X < bp->X) ret = false;
	else if (ap->Y < bp->Y) ret = true;
	else if (ap->Y > bp->Y) ret = false;
	else if (ap->Z < bp->Z) ret = true;
	else if (ap->Z > bp->Z) ret = false;
	else if (at < bt) ret = true;

	return ret;
}
