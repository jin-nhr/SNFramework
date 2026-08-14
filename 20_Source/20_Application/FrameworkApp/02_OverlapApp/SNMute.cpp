#include "SNMute.h"
#include "SNGraphics.h"
#include "SNSystemConfig.h"
#include "SNMath.h"
#include "SNGraphicsDevice.h"

SNBitmap SNMute::Surface;
Boolean SNMute::NowMuteSts = false;
Float32 SNMute::Alpha = 0;
Boolean SNMute::RequestMute = false;
Boolean SNMute::RequestFade = false;

SNTimerSequencer SNMute::FadeInSeq;
SNTimerSequencer SNMute::FadeOutSeq;


// ミュート設定(on/off, fade有無, 色)
Void SNMute::SetMute(Boolean mute, Boolean fade)
{
	RequestMute = mute;
	RequestFade = fade;
	
	return;
}

Boolean SNMute::GetNowSts()
{
	return NowMuteSts;
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

	FadeInSeq.Initialize(this, 0, 3);
	FadeInSeq.SetWait(0, 0);
	FadeInSeq.SetWait(1, 0);
	FadeInSeq.SetWait(2, 0);

	FadeOutSeq.Initialize(this, 1, 3);
	FadeOutSeq.SetWait(0, 0);
	FadeOutSeq.SetWait(1, 0);
	FadeOutSeq.SetWait(2, 0);

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
	NowMuteSts = false;
	Alpha = SNAlphaMin;;
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
	// フェードイン中
	if (FadeInSeq.IsProc())
	{
		FadeInSeq.Step();
	}

	// フェードアウト中
	else if (FadeOutSeq.IsProc())
	{
		FadeOutSeq.Step();
	}

	else
	{
		// ミュート状態変化確認
		if (NowMuteSts != RequestMute)
		{
			if (RequestFade)
			{
				if (RequestMute)
				{
					// フェードアウト
					FadeOutSeq.Start();
				}
				else
				{
					// フェードイン
					FadeInSeq.Start();
				}
			}

			// フェードなし
			else
			{
				// ミュート
				if (RequestMute)
				{
					Visible = true;
					Alpha = SNAlphaMax;
				}
				// ミュート解除
				else
				{
					Visible = false;
				}
				// 状態更新
				NowMuteSts = RequestMute;
			}
		}
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

SNPhaseResult SNMute::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;
	Float32 fade_step;
	Float32 tmp_alpha;

	switch (ch)
	{
	// Fade In
	case 0:
		switch (phase_idx)
		{
		case 0:
			Visible = true;
			Alpha = SNAlphaMax;
			ret = SNPhaseResultNext;
			break;
		case 1:
			fade_step = SNMath::SaturateF(SNAlphaMax / (SNSystemConfig::FadeTime / (1000.0F / SNSystemConfig::FPS)), SNFadeStepMin, SNFadeStepMax);
			tmp_alpha = Alpha - fade_step;
			Alpha = SNMath::SaturateF(tmp_alpha, SNAlphaMin, SNAlphaMax);

			// フェードイン完了時
			if (tmp_alpha <= SNAlphaMin)
			{
				ret = SNPhaseResultNext;
			}
			break;
		case 2:
			Visible = false;
			NowMuteSts = false;
			ret = SNPhaseResultNext;
			break;
		}
		break;

	// Fade Out
	case 1:
		switch (phase_idx)
		{
		case 0:
			Visible = true;
			Alpha = SNAlphaMin;
			ret = SNPhaseResultNext;
			break;
		case 1:
			fade_step = SNMath::SaturateF(SNAlphaMax / (SNSystemConfig::FadeTime / (1000.0F / SNSystemConfig::FPS)), SNFadeStepMin, SNFadeStepMax);
			tmp_alpha = Alpha + fade_step;
			Alpha = SNMath::SaturateF(tmp_alpha, SNAlphaMin, SNAlphaMax);

			// フェードアウト完了時
			if (tmp_alpha >= SNAlphaMax)
			{
				ret = SNPhaseResultNext;
			}
			break;
		case 2:
			Visible = true;
			NowMuteSts = true;
			ret = SNPhaseResultNext;
			break;
		}
		break;
	}

	return ret;
}

