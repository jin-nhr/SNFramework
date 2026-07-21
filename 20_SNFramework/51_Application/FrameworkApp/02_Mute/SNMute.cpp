#include "SNMute.h"
#include "SNGraphics.h"
#include "SNSystemConfig.h"
#include "SNMath.h"
#include "SNGraphicsDevice.h"

SNBitmap SNMute::Surface;
SNMuteSts SNMute::NowSts = SNMuteStsOn;
Boolean SNMute::UpdateFlag = false;
Float32 SNMute::Alpha = 0;
Float32 SNMute::FadeStep = SNFadeStepMin;
Boolean SNMute::RequestMute = false;
Boolean SNMute::RequestFade = false;

// ミュート設定(on/off, fade有無, 色)
Void SNMute::SetMute(Boolean mute, Boolean fade)
{
	RequestMute = mute;
	RequestFade = fade;

	UpdateFlag = true;
	
	return;
}

// コンストラクタ
SNMute::SNMute()
{
	return;
}

// デストラクタ
SNMute::~SNMute()
{
	return;
}

// 初期化
Void SNMute::OnInitialize()
{
	SNSize size;
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNColor color = { 0, 0, 0, 255 };	// 黒

	size.Width = SNSystemConfig::ScreenWidth;
	size.Height = SNSystemConfig::ScreenHeight;

	// 画面サイズのサーフェス生成
	grc->CreateBitmap(&Surface, &size);

	// 初期設定
	grc->Begin(&Surface);
	grc->Clear(&color);
	grc->End();

	return;
}

	// 終了処理
Void SNMute::OnTerminate()
{
	Surface.DeleteBitmap();

	return;
}

// Entry
Void SNMute::OnEntry()
{
	// 初期値として非表示にしておく
	Visible = false;
	NowSts = SNMuteStsOn;
	UpdateFlag = false;
	Alpha = 0;
	FadeStep = SNFadeStepMin;
	RequestMute = false;
	RequestFade = false;

	return;
}

	// Exit
Void SNMute::OnExit()
{
	return;
}

	// フレーム処理
Void SNMute::OnCycle()
{
	SNMuteSts prev_sts = NowSts;

	// 更新あり
	if (UpdateFlag)
	{
		// FadeOut
		if (RequestMute && RequestFade)
		{
			NowSts = SNMuteStsFadeOut;
			Visible = true;
			Alpha = SNAlphaMin;
			FadeStep = (Float32)SNAlphaMax / (Float32)(SNSystemConfig::FadeTime / (1000.0F / SNSystemConfig::FPS));

		}
		// Mute
		else if (RequestMute && !RequestFade)
		{
			NowSts = SNMuteStsOn;
			Alpha = SNAlphaMax;
			FadeStep = SNFadeStepMax;
			Visible = true;
		}
		// FadeIn
		else if (!RequestMute && RequestFade)
		{
			NowSts = SNMuteStsFadeIn;
			Visible = true;
			Alpha = SNAlphaMax;
			FadeStep = -1 * (Float32)SNAlphaMax / (Float32)(SNSystemConfig::FadeTime / (1000.0F / SNSystemConfig::FPS));

		}
		// Mute Off
		else
		{
			NowSts = SNMuteStsOff;
			Alpha = SNAlphaMin;
			FadeStep = SNFadeStepMin;
			Visible = false;
		}

		UpdateFlag = false;
	}

	// フェード処理継続中
	switch (NowSts)
	{
	case SNMuteStsFadeOut:
	case SNMuteStsFadeIn:
		// フェード処理中
		if (prev_sts == NowSts)
		{
			Alpha += FadeStep;
			Alpha = (Float32)SNMath::SelectMax((Int64)Alpha, SNAlphaMin);
			Alpha = (Float32)SNMath::SelectMin((Int64)Alpha, SNAlphaMax);
		}
		break;
	}

	return;
}

// 描画処理
Void SNMute::OnDraw(SNGraphicsContext* grc)
{
	SNSize size;
	SNRect dst_rect;
	SNRect src_rect;

	Surface.GetSize(&size);

	dst_rect.PointX = 0;
	dst_rect.PointY = 0;
	dst_rect.Width = SNSystemConfig::ScreenWidth;
	dst_rect.Height = SNSystemConfig::ScreenHeight;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = size.Width;
	src_rect.Height = size.Height;

	grc->DrawImage(&dst_rect, &Surface, &src_rect, (UInt8)Alpha);

	return;
}
