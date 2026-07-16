#include "SNGraphics.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNGraphicsDevice.h"
#include "SNApplication.h"
#include "SNImageCodec.h"
#include "SNGraphicsResManager.h"
#include "SNGraphicsResource.h"

// グラフィクスクラス
// SNFrameworkにおける描画の制御を行う

// 前フレームフルスクリーン状態
Boolean SNGraphics::PreFullScreenSts = false;

// 前フレームウインドウサイズ
SNSize SNGraphics::PreWindowSize = {0};

// 描画矩形データ
SNRect SNGraphics::DrawRect = { 0 };


// 初期化処理
Void SNGraphics::Initialize()
{
	// GDI初期化

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

	SNGraphicsDevice::Initialize();
	SNGraphicsResManager::Initialize();

	StartUpResourceLoad();

	// ビットマップフォント初期化
	SNBitmapFont::Initialize();

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

	SNGraphicsResManager::Terminate();
	SNGraphicsDevice::Terminate();

	return;
}

// 終了
Void SNGraphics::Terminate()
{
	return;
}

// システムリソースのロード
// ワーカースレッドからの呼び出しとなるため
// GDIロックが必要
Void SNGraphics::LoadSystemResource()
{

	return;
}

// 更新
Void SNGraphics::Update()
{
	Boolean is_full;
	SNSize win_size;

	// リソース管理の更新
	SNGraphicsResManager::Update();

	// 最新フレームの情報取得
	is_full = IsFullScreen();

	// フルスクリーン状態更新
	SNGraphicsDevice::SetFullScreen(is_full);

	// ウインドウサイズ取得
	SNGraphicsDevice::GetWindowSize(&win_size);

	// DrawRect更新
	UpdateDrawRect(&win_size);

	// デバイスリストア処理
	DeviceRestore(is_full, &win_size);

	// 前フレーム情報更新
	UpdatePreFrameInfo(is_full, &win_size);

	return;
}

// サーフェスフリップ
Void SNGraphics::FlipSurface()
{
	SNGraphicsDevice::Flip(&DrawRect);

	return;
}

// 描画領域更新
Void SNGraphics::UpdateDrawRect(SNSize* size)
{
	Int32 width;
	Int32 height;
	UInt8 draw_align = SNSystemConfig::DrawAlign;
	Int32 config_width = SNSystemConfig::ScreenWidth;
	Int32 config_height = SNSystemConfig::ScreenHeight;

	width = size->Width;
	height = size->Height;

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

Void SNGraphics::DeviceRestore(Boolean is_full, SNSize* size)
{
	// フルスクリーン状態の更新またはウインドウサイズの変更あり
	if ((is_full != PreFullScreenSts) ||
		(size->Width != PreWindowSize.Width) ||
		(size->Height != PreWindowSize.Height))
	{
		SNGraphicsDevice::Restore(size);
	}

	return;
}

Void SNGraphics::UpdatePreFrameInfo(Boolean is_full, SNSize* size)
{
	// 前フレーム状態更新
	PreFullScreenSts = is_full;
	PreWindowSize = *size;

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

// コンテキスト取得
SNGraphicsContext* SNGraphics::GetContext()
{
	SNGraphicsContext* dc = &SNGraphicsDevice::D2DGraphicsContext;
	SNBitmap* tgbmp = &SNGraphicsDevice::D2DTargetBitmap;
	SNColor color = { 0, 0, 0, 255 };

	// 描画開始
	dc->Begin(tgbmp);
	dc->Clear(&color);

	return dc;
}

// コンテキスト解放
Void SNGraphics::ReleaseContext()
{
	SNGraphicsContext* dc = &SNGraphicsDevice::D2DGraphicsContext;

	dc->End();
	return;
}

// スタートアップ用のリソース読み込み
Void SNGraphics::StartUpResourceLoad()
{
	Int32 cnt;

	// スタートアップ処理のロードを実行
	for (cnt = SNGraphicsResStartupTop; cnt <= SNGraphicsResStartupEnd; cnt++)
	{
		SNGraphicsResManager::AccessGet((SNGraphicsResID)cnt);
	}

	// スタートアップ処理のロードを実行
	for (cnt = SNGraphicsResStartupTop; cnt <= SNGraphicsResStartupEnd; cnt++)
	{
		// 処理完了までループ
		while (!SNGraphicsResManager::IsLoaded((SNGraphicsResID)cnt))
		{
			// 更新を繰り返し完了を待つ
			SNGraphicsResManager::Update();
			::Sleep(1);
		}
	}

	return;
}
