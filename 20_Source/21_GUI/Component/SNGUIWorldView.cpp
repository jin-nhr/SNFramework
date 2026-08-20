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
	if (ViewScale >= 1.0f)
	{
		ViewScale += SNWViewScaleStepHigh;
	}
	else
	{
		ViewScale += SNWViewScaleStepLow;
	}

	return;
}

Void SNGUIWorldView::DownViewScale()
{
	if (ViewScale > 1.0f)
	{
		ViewScale -= SNWViewScaleStepHigh;
	}
	else
	{
		ViewScale -= SNWViewScaleStepLow;
	}

	return;
}

Void SNGUIWorldView::GetViewPos(SNWorldPos* pos)
{
	*pos = TargetPos;

	return;
}

Void SNGUIWorldView::OnInitialize()
{
	SNSize work_size;
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;

	work_size.Width = (Int32)(SNSystemConfig::ScreenWidth / SNWViewScaleMin);
	work_size.Height = (Int32)(SNSystemConfig::ScreenHeight / SNWViewScaleMin);

	grc->CreateBitmap(&WorkSurface, &work_size);

	return;
}

Void SNGUIWorldView::SetViewDir(SNWorldDir dir)
{
	ViewDir = dir;

	return;
}

Void SNGUIWorldView::RotateRViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Increment(ViewDir + 1, SNWorldDirN, SNWorldDirNW);

	return;
}

Void SNGUIWorldView::RotateLViewDir()
{
	ViewDir = (SNWorldDir)SNMath::Decrement(ViewDir - 1, SNWorldDirN, SNWorldDirNW);

	return;
}

SNWorldDir SNGUIWorldView::GetViewDir()
{
	return ViewDir;
}


Void SNGUIWorldView::OnPreDraw()
{
	SNWorld::Draw(&WorkSurface, &TargetPos, ViewDir);

	return;
}

Void SNGUIWorldView::OnDraw(SNGraphicsContext *grc)
{
	SNRect dst_rect;
	SNRect src_rect;
	SNSize work_size;

	WorkSurface.GetSize(&work_size);

	dst_rect = GetRect();

	src_rect.Width = (Int32)(SNSystemConfig::ScreenWidth / ViewScale);
	src_rect.Height = (Int32)(SNSystemConfig::ScreenHeight / ViewScale);

	src_rect.PointX = (work_size.Width - src_rect.Width) / 2; 
	src_rect.PointY = (work_size.Height - src_rect.Height) / 2;

	grc->DrawImage(&dst_rect, &WorkSurface, &src_rect, SNAlphaMax);

	return;
}
