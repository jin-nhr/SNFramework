#include "SNGraphics.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNAutoResource.h"
#include "SNGDI.h"
#include "SNSystemColorTable.h"
#include "SNSystemPen.h"
#include "SNSystemBrush.h"
#include "SNSystemSurface.h"
#include "SND3D.h"
#include "SND2D.h"
#include "SNApplication.h"

// グラフィクスクラス
// SNFrameworkにおける描画の制御を行う

// 前フレームフルスクリーン状態
Boolean SNGraphics::PreFullScreenSts = false;

// 前フレームウインドウサイズ
SNSize SNGraphics::PreWindowSize = {0};

// 描画矩形データ
SNRect SNGraphics::DrawRect = { 0 };

// サーフェス
SNSurfaceD3D SNGraphics::Surface;

// 初期化処理
Void SNGraphics::Initialize()
{
	// GDI初期化
	SNGDI::Initialize();

	return;
}

// 起動準備
Void SNGraphics::Startup()
{
	// 描画領域を初期設定
	DrawRect.PointX = 0;
	DrawRect.PointY = 0;
	DrawRect.Width = SNSystemConfig::ScreenWidth;
	DrawRect.Height = SNSystemConfig::ScreenHeight;;

	// D2D, D3D初期化
	SND3D::CreateDevice();
	SND3D::CreateSwapChain();
	SND3D::CreateRTV();
	SND3D::CreateSurface();
	SND2D::Initialize();
	SND3D::CreateSRV();
	SND3D::CreateFullscreenQuad();
	SND3D::CreateShaders();
	SND3D::CreateSampler();

	SNSystemColorTable::Initialize();

	return;
}

// 実行
Void SNGraphics::Run()
{
	return;
}

// 終了前処理
Void SNGraphics::BeforeTerminate()
{
	// ビットマップフォント終了
	SNBitmapFont::Terminate();

	SNSystemSurface::Terminate();
	SNSystemPen::Terminate();
	SNSystemBrush::Terminate();
	SNSystemColorTable::Terminate();

	// D2D, D3D関連
	SND2D::Terminate();
	SND3D::ReleaseSampler();
	SND3D::ReleaseShaders();
	SND3D::ReleaseFullscreenQuad();
	SND3D::ReleaseSRV();
	SND3D::ReleaseSurface();
	SND3D::ReleaseRTV();
	SND3D::ReleaseSwapChain();
	SND3D::ReleaseDevice();

	return;
}

// 終了
Void SNGraphics::Terminate()
{
	// GDI終了
	SNGDI::Terminate();

	return;
}

// システムリソースのロード
// ワーカースレッドからの呼び出しとなるため
// GDIロックが必要
Void SNGraphics::LoadSystemResource()
{
	SNSystemPen::Initialize();
	SNSystemBrush::Initialize();
	SNSystemSurface::Initialize();

	// ビットマップフォント初期化
	SNBitmapFont::Initialize();
	return;
}

// 更新
Void SNGraphics::Update()
{
	Boolean is_full;
	RECT rect;
	SNRect snrect = {0};
	SNSize win_size;

	// 最新フレームの情報取得
	is_full = IsFullScreen();

	// フルスクリーン状態の更新
	if (is_full != PreFullScreenSts)
	{
		SND3D::SetFullScreen(is_full);
	}

	if (is_full)
	{
		SND3D::GetScreenRect(&snrect);
	}
	else
	{
		::GetClientRect((HWND)SNWindow::WindowHandle, &rect);
		snrect.Width = rect.right;
		snrect.Height = rect.bottom;
	}

	win_size.Width = snrect.Width;;
	win_size.Height = snrect.Height;

	// DrawRect更新
	UpdateDrawRect(&snrect);

	// フルスクリーン状態の更新またはウインドウサイズの変更あり
	if ((is_full != PreFullScreenSts) ||
		(win_size.Width != PreWindowSize.Width) ||
		(win_size.Height != PreWindowSize.Height))
	{
		// RTVの破棄
		SND3D::ReleaseRTV();
		// D2Dの破棄
		
		// バックバッファ再構築
		SND3D::ResizeBuffer(&win_size);

		// RTVの再生成
		SND3D::CreateRTV();

		// D2Dの再構築
	}

	// 前フレーム状態更新
	PreFullScreenSts = is_full;
	PreWindowSize = win_size;

	return;
}

// サーフェスフリップ
Void SNGraphics::FlipSurface()
{
	SND2D::Draw();
	SND3D::Flip(&DrawRect);

	return;
}

SNSurfaceD3D* SNGraphics::GetSurface()
{
	return &Surface;
}

// 画面描画処理
Void SNGraphics::UpdateDrawRect(SNRect* rect)
{
	Int32 width;
	Int32 height;
	UInt8 draw_align = SNSystemConfig::DrawAlign;
	Int32 config_width = SNSystemConfig::ScreenWidth;
	Int32 config_height = SNSystemConfig::ScreenHeight;

	width = rect->Width;
	height = rect->Height;

	// アスペクト比を維持したスケール計算
	Float32 scale_x = (Float32)width / config_width;
	Float32 scale_y = (Float32)height / config_height;
	Float32 scale = min(scale_x, scale_y);  // アスペクト比を維持

	Int32 new_width = (Int32)(config_width * scale);
	Int32 new_height = (Int32)(config_height * scale);

	// 4の倍数に補正
	new_width = (new_width / draw_align) * draw_align;
	new_height = (new_height / draw_align) * draw_align;

	// 描画領域のオフセット計算（中央配置）
	Int32 offset_x = (width - new_width) / 2;
	Int32 offset_y = (height - new_height) / 2;

	// 描画範囲を更新
	DrawRect.PointX = offset_x;
	DrawRect.PointY = offset_y;
	DrawRect.Width = new_width;
	DrawRect.Height = new_height;

	return;
}

// 画面座標系→サーフェス座標に変換
Boolean SNGraphics::ClientToSurface(SNPoint* point)
{
	SNRect draw_rect;
	Boolean clipping = false;

	// 描画範囲を参照用にコピー
	draw_rect = DrawRect;

	// X座標が描画左端より左なら左端でクリップ
	if (point->X < draw_rect.PointX)
	{
		point->X = draw_rect.PointX;
		clipping = true;
	}
	// X座標が描画右端より右なら右端でクリップ
	if (draw_rect.PointX + draw_rect.Width <= point->X)
	{
		point->X = draw_rect.PointX + draw_rect.Width - 1;
		clipping = true;
	}
	// Y座標が描画上端より上なら上でクリップ
	if (point->Y < draw_rect.PointY)
	{
		point->Y = draw_rect.PointY;
		clipping = true;
	}
	// Y座標が描画下端より下なら下でクリップ
	if (draw_rect.PointY + draw_rect.Height <= point->Y)
	{
		point->Y = draw_rect.PointY + draw_rect.Height - 1;
		clipping = true;
	}

	// 0割回避
	if (draw_rect.Width == 0) draw_rect.Width = 1;
	if (draw_rect.Height == 0) draw_rect.Height = 1;

	// 描画範囲でクリップした座標をサーフェス座標系にマッピング
	point->X = ((point->X - draw_rect.PointX) * SNSystemConfig::ScreenWidth) / draw_rect.Width;
	point->Y = ((point->Y - draw_rect.PointY) * SNSystemConfig::ScreenHeight) / draw_rect.Height;

	return clipping;
}

// フルスクリーン判定
Boolean SNGraphics::IsFullScreen()
{
	return ((SNUserConfig::Data.FullScreen) & (SNApplication::Active));
}
