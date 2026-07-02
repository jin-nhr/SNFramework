#include "SNGUIFader.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"
#include "SNMath.h"
#include "SNSystemSurface.h"

// フェーダークラス

// コンストラクタ
SNGUIFader::SNGUIFader()
{
	// 変数初期化
	FadeStep = 0;
	Alpha = 0;
	TriggerIn = false;
	TriggerOut = false;
	FadeState = SNGUIFadeStateIdle;
	Surface = &SNSystemSurface::Surface[SNColorBlack];
	FadeType = SNGUIFadeTypeCloss;

	return;
}

// デストラクタ
SNGUIFader::~SNGUIFader()
{
	return;
}

// 初期化
Void SNGUIFader::OnInitialize()
{
	return;
}

// 終了処理
Void SNGUIFader::OnTerminate()
{
	return;
}

// フェード種別設定
Void SNGUIFader::SetFadeType(SNGUIFadeType fadetype)
{
	FadeType = fadetype;

	return;
}

// 色設定
// 基本色サーフェスを設置
Void SNGUIFader::SetColor(SNColorCode color)
{
	Surface = &SNSystemSurface::Surface[color];

	return;
}

// サーフェス設定
// 任意サーフェスを設定
Void SNGUIFader::SetSurface(SNSurface* surface)
{
	Surface = surface;

	return;
}

// フェードイン
// パラメータ：待ち時間をミリ秒指定(最大約65秒)
Void SNGUIFader::FadeIn(UInt16 msec)
{
	UInt32 fps = SNSystemConfig::FPS;
	Int32 step = 0;

	// 1フレームあたりの変異量計算
	step = (255 * fps) / msec;

	step = (Int32)SNMath::SelectMax(step, SNFadeStepMin);
	step = (Int32)SNMath::SelectMin(step, SNFadeStepMax);
	FadeStep = (UInt8)step;

	Alpha = 255;
	TriggerIn = true;
	FadeState = SNGUIFadeStateIdle;

	return;
}

// フェードアウト
// パラメータ：待ち時間をミリ秒指定(最大約65秒)
Void SNGUIFader::FadeOut(UInt16 msec)
{
	UInt32 fps = SNSystemConfig::FPS;
	Int32 step = 0;

	// 1フレームあたりの変異量計算
	step = (255 * fps) / msec;

	step = (Int32)SNMath::SelectMax(step, SNFadeStepMin);
	step = (Int32)SNMath::SelectMin(step, SNFadeStepMax);
	FadeStep = (UInt8)step;

	Alpha = 0;
	TriggerOut = true;
	FadeState = SNGUIFadeStateIdle;

	return;
}

// アルファ値設定
// フェード指示前の状態を指定したい場合に設定
Void SNGUIFader::SetAlpha(UInt8 alpha)
{
	Alpha = alpha;
}

// 処理状態取得
Boolean SNGUIFader::IsProcessing()
{
	return ((FadeState == SNGUIFadeStateIn) || (FadeState == SNGUIFadeStateOut));
}

// 完了状態取得
Boolean SNGUIFader::IsComplete()
{
	return (FadeState == SNGUIFadeStateComplete);
}

// Step実行
SNTransitionCode SNGUIFader::OnStep(SNEvent* event)
{
	Int32 alpha;

	switch (FadeState)
	{
	case SNGUIFadeStateIdle:
		// フェードイントリガあり
		if (TriggerIn)
		{
			FadeState = SNGUIFadeStateIn;
			TriggerIn = false;
		}

		// フェードアウトトリガあり
		else if (TriggerOut)
		{
			FadeState = SNGUIFadeStateOut;
			TriggerOut = false;
		}
		break;

	case SNGUIFadeStateIn:
		alpha = Alpha - FadeStep;
		alpha = (Int32)SNMath::SelectMax(alpha, SNAlphaMin);
		alpha = (Int32)SNMath::SelectMin(alpha, SNAlphaMax);
		Alpha = (UInt8)alpha;

		// Alpha=Minまで進んだら完了
		if (Alpha == SNAlphaMin)
		{
			FadeState = SNGUIFadeStateComplete;
		}
		break;

	case SNGUIFadeStateOut:
		alpha = Alpha + FadeStep;
		alpha = (Int32)SNMath::SelectMax(alpha, SNAlphaMin);
		alpha = (Int32)SNMath::SelectMin(alpha, SNAlphaMax);
		Alpha = (UInt8)alpha;

		// Alpha=最大まで進んだら完了
		if (Alpha == SNAlphaMax)
		{
			FadeState = SNGUIFadeStateComplete;
		}
		break;

	case SNGUIFadeStateComplete:
		break;
	default:
		break;
	}

	return SNTransitionCodeStay;
}

// 描画処理
Void SNGUIFader::OnDraw(SNSurface* surface)
{
	// フェード種別毎の描画
	DrawFadeEffect(surface);

	return;
}

// フェード種別毎の描画処理
Void SNGUIFader::DrawFadeEffect(SNSurface* surface)
{
	SNRect rect = CalcGlobalRect();

	// StepがZero
	if (Alpha == SNAlphaMin)
	{
		// AlphaがMinのときは描画しない
	}

	// Alphaが中間値
	else if (Alpha < SNAlphaMax)
	{
		switch (FadeType)
		{
		case SNGUIFadeTypeCloss:
			// アルファブレンド
			surface->AlphaBlend(
				rect.PointX,
				rect.PointY,
				rect.Width,
				rect.Height,
				(Handle)Surface->GetDC(),
				0,
				0,
				Surface->GetWidth(),
				Surface->GetHeight(),
				Alpha,
				false);
			break;

		case SNGUIFadeTypeFromLeft:
			// 右描画
			DrawFadeEffectRight(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeFromRight:
			// 左描画
			DrawFadeEffectLeft(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeFromTop:
			// 下描画
			DrawFadeEffectBottom(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeFromBottom:
			// 上描画
			DrawFadeEffectTop(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeFromSide:
			// 横方向中央
			DrawFadeEffectCenterH(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeCenterToSide:
			// 左描画 (step半分)
			DrawFadeEffectLeft(surface, Alpha / 2, &rect);
			// 右描画 (step半分)
			DrawFadeEffectRight(surface, Alpha / 2, &rect);
			break;

		case SNGUIFadeTypeFromTopBottom:
			// 縦方向中央
			DrawFadeEffectCenterV(surface, Alpha, &rect);
			break;

		case SNGUIFadeTypeCenterToTopBottom:
			// 上描画 (step半分)
			DrawFadeEffectTop(surface, Alpha / 2, &rect);
			// 下描画 (step半分)
			DrawFadeEffectBottom(surface, Alpha / 2, &rect);
			break;
		}
	}

	// alpha最大
	else
	{
		// サーフェスで塗りつぶし
		surface->StretchBlt(
			rect.PointX,
			rect.PointY,
			rect.Width,
			rect.Height,
			(Handle)Surface->GetDC(),
			0,
			0,
			Surface->GetWidth(),
			Surface->GetHeight());
	}

	return;
}

// 左描画
Void SNGUIFader::DrawFadeEffectLeft(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_width;
	Int32 src_width;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 左からの描画時は、描画幅をalphaに応じて調整
	
	dst_width = dst_rect->Width * alpha / SNFadeStepMax;
	src_width = src_rect.Width * alpha / SNFadeStepMax;

	surface->StretchBlt(
		dst_rect->PointX,
		dst_rect->PointY,
		dst_width,
		dst_rect->Height,
		Surface->GetDC(),
		src_rect.PointX,
		src_rect.PointY,
		src_width,
		src_rect.Height);

	return;
}

// 右描画
Void SNGUIFader::DrawFadeEffectRight(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_width;
	Int32 src_width;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 右からの描画時は、X座標と幅をalphaに応じて調整

	dst_width = dst_rect->Width * alpha / SNFadeStepMax;
	src_width = src_rect.Width * alpha / SNFadeStepMax;

	surface->StretchBlt(
		dst_rect->PointX + dst_rect->Width - dst_width,
		dst_rect->PointY,
		dst_width,
		dst_rect->Height,
		Surface->GetDC(),
		src_rect.PointX + src_rect.Width - src_width,
		src_rect.PointY,
		src_width,
		src_rect.Height);

	return;
}

// 横方向中央描画
Void SNGUIFader::DrawFadeEffectCenterH(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_width;
	Int32 src_width;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 横方向中央描画時は、X座標、幅をalphaに応じて調整

	dst_width = dst_rect->Width * alpha / SNFadeStepMax;
	src_width = src_rect.Width * alpha / SNFadeStepMax;

	surface->StretchBlt(
		(dst_rect->PointX + dst_rect->Width / 2) - dst_width / 2,
		dst_rect->PointY,
		dst_width,
		dst_rect->Height,
		Surface->GetDC(),
		(src_rect.PointX + src_rect.Width / 2) - src_width / 2,
		src_rect.PointY,
		src_width,
		src_rect.Height);

	return;
}

// 上描画
Void SNGUIFader::DrawFadeEffectTop(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_height;
	Int32 src_height;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 上からの描画時は、高さをalphaに応じて調整

	dst_height = dst_rect->Height * alpha / SNFadeStepMax;
	src_height = src_rect.Height * alpha / SNFadeStepMax;

	surface->StretchBlt(
		dst_rect->PointX,
		dst_rect->PointY,
		dst_rect->Width,
		dst_height,
		Surface->GetDC(),
		src_rect.PointX,
		src_rect.PointY,
		src_rect.Width,
		src_height);

	return;
}

// 下描画
Void SNGUIFader::DrawFadeEffectBottom(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_height;
	Int32 src_height;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 下からの描画時は、Y座標、高さをalphaに応じて調整

	dst_height = dst_rect->Height * alpha / SNFadeStepMax;
	src_height = src_rect.Height * alpha / SNFadeStepMax;

	surface->StretchBlt(
		dst_rect->PointX,
		dst_rect->PointY + dst_rect->Height - dst_height,
		dst_rect->Width,
		dst_height,
		Surface->GetDC(),
		src_rect.PointX,
		src_rect.PointY + src_rect.Height - src_height,
		src_rect.Width,
		src_height);

	return;

}

// 縦方向中央描画
Void SNGUIFader::DrawFadeEffectCenterV(SNSurface* surface, UInt8 alpha, SNRect* dst_rect)
{
	SNRect src_rect;
	Int32 dst_height;
	Int32 src_height;

	src_rect.PointX = 0;
	src_rect.PointY = 0;
	src_rect.Width = Surface->GetWidth();
	src_rect.Height = Surface->GetHeight();

	// 縦方向中央描画時は、Y座標と高さをalphaに応じて調整

	dst_height = dst_rect->Height * alpha / SNFadeStepMax;
	src_height = src_rect.Height * alpha / SNFadeStepMax;

	surface->StretchBlt(
		dst_rect->PointX,
		(dst_rect->PointY + dst_rect->Height / 2) - dst_height / 2,
		dst_rect->Width,
		dst_height,
		Surface->GetDC(),
		src_rect.PointX,
		(src_rect.PointY + src_rect.Height / 2) - src_height / 2,
		src_rect.Width,
		src_height);

	return;
}

