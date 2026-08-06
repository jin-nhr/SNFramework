#include "SNGraphics.h"
#include "SNWindowsAPI.h"
#include "SNSystem.h"
#include "SNConfig.h"
#include "SNBitmapFont.h"
#include "SNGraphicsDevice.h"
#include "SNApplication.h"
#include "SNImageCodec.h"
#include "SNScaling.h"

// グラフィクスクラス
// SNFrameworkにおける描画の制御を行う

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

	LoadStartupResource();

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
	DeviceRestore(&win_size);

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
	SNRect src_rect = { 0, 0, SNSystemConfig::ScreenWidth, SNSystemConfig::ScreenHeight };
	SNRect dst_rect = { 0, 0, size->Width, size->Height };

	SNScaling::LetterBox(&src_rect, &dst_rect);

	DrawRect = dst_rect;

	return;
}

Void SNGraphics::DeviceRestore(SNSize* size)
{
	// ウインドウのサイズ変更通知をトリガにして
	// デバイスのリストアを実行する
	if (SNApplication::GetEventInfo(SNEventWindowSize))
	{
		SNGraphicsDevice::Restore(size);
	}

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
Void SNGraphics::LoadStartupResource()
{
	// スタートアップ処理のロードを実行
	LoadResource(SNGraphicsResStartupTop, SNGraphicsResStartupEnd);

	// 処理完了までループ
	while (!IsResourceLoaded(SNGraphicsResStartupTop, SNGraphicsResStartupEnd))
	{
		SNGraphicsResManager::Update();
		Sleep(1);
	}

	return;
}

// システムリソースロード
Void SNGraphics::LoadSystemResource()
{
	LoadResource(SNGraphicsResSystemTop, SNGraphicsResSystemEnd);

	SNBitmapFont::InitializeExtraFunc();

	return;
}

// システムリソースロード完了判定
Boolean SNGraphics::IsSystemResourceLoaded()
{
	return IsResourceLoaded(SNGraphicsResSystemTop, SNGraphicsResSystemEnd);
}

// システムリソースアンロード
Void SNGraphics::UnloadSystemResource()
{
	UnloadResource(SNGraphicsResSystemTop, SNGraphicsResSystemEnd);
	return;
}

// アプリリソースロード
Void SNGraphics::LoadAppResource()
{
	LoadResource(SNGraphicsResAppTop, SNGraphicsResAppEnd);
	return;
}

// アプリリソースロード完了判定
Boolean SNGraphics::IsAppResourceLoaded()
{
	return IsResourceLoaded(SNGraphicsResAppTop, SNGraphicsResAppEnd);
}

// アプリリソースアンロード
Void SNGraphics::UnloadAppResource()
{
	UnloadResource(SNGraphicsResAppTop, SNGraphicsResAppEnd);
	return;
}

Void SNGraphics::LoadResource(SNGraphicsResID st_id, SNGraphicsResID ed_id)
{
	Int32 res_id;

	for (res_id = st_id; res_id <= ed_id; res_id++)
	{
		SNGraphicsResManager::AccessGet((SNGraphicsResID)res_id);
	}
	return;
}

Boolean SNGraphics::IsResourceLoaded(SNGraphicsResID st_id, SNGraphicsResID ed_id)
{
	Int32 res_id;
	Boolean loaded = true;

	for (res_id = st_id; res_id <= ed_id; res_id++)
	{
		loaded &= SNGraphicsResManager::IsLoaded((SNGraphicsResID)res_id);
	}

	return loaded;
}
Void SNGraphics::UnloadResource(SNGraphicsResID st_id, SNGraphicsResID ed_id)
{
	Int32 res_id;

	for (res_id = st_id; res_id <= ed_id; res_id++)
	{
		SNGraphicsResManager::AccessRelease((SNGraphicsResID)res_id);
	}

	return;
}
