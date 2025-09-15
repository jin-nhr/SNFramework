#include "SNGraphics.h"
#include "../System/SNWindowsAPI.h"
#include "../System/SNSystem.h"
#include "../Configuration/SNConfiguration.h"


// グラフィクスクラス

// 画面バッファ用クリティカルセクション
SNCriticalSection SNGraphics::CriticalSectionForScreen;

// 描画矩形データ用クリティカルセクション
SNCriticalSection SNGraphics::CriticalSectionForDrawRect;

// 画面サーフェス
SNSurfaceDDB SNGraphics::ScreenSurface[2];

// プライマリサーフェスインデックス
Int32 SNGraphics::PrimaryIndex = 0;

// セカンダリサーフェスインデックス
Int32 SNGraphics::SecondaryIndex = 1;

// 描画対象画面サイズ
SNRect SNGraphics::ScreenRect = { 0 };;

// 描画矩形データ
SNRect SNGraphics::DrawRect = { 0 };


// 初期化処理
Void SNGraphics::Initialize()
{
	// クリティカルセクション初期化
	CriticalSectionForScreen.Initialize();
	CriticalSectionForDrawRect.Initialize();

	return;
}

// 起動準備
Void SNGraphics::Startup()
{
	Int32 width = SNConfiguration::SystemConfiguration.ScreenWidth;
	Int32 height = SNConfiguration::SystemConfiguration.ScreenHeight;

	// サーフェス生成
	ScreenSurface[0].CreateSurface(width, height);
	ScreenSurface[1].CreateSurface(width, height);

	// 描画領域を初期設定
	DrawRect.PointX = 0;
	DrawRect.PointY = 0;
	DrawRect.Width = width;
	DrawRect.Height = height;

	return;
}

// 実行
// リターン：終了コード
Int32 SNGraphics::Run()
{
	return 0;
}

// 終了前処理
Void SNGraphics::BeforeTerminate()
{
	return;
}

// 終了
Void SNGraphics::Terminate()
{
	return;
}

// サーフェス取得
SNSurface* SNGraphics::GetSurface()
{
	// セカンダリサーフェスを返す
	return &ScreenSurface[SecondaryIndex];
}

// サーフェスフリップ
Void SNGraphics::FlipSurface()
{
	// 描画処理はプライマリスレッドで実行され、
	// サーフェスフリップはアプリケーションスレッドで実行するため
	// サーフェスへのアクセス競合が発生する懸念があるため排他制御を行う
	// クリティカルセクションロック
	CriticalSectionForScreen.Lock();
	{
		// サーフェスを入れ替え
		Int32 swap_index = PrimaryIndex;
		PrimaryIndex = SecondaryIndex;
		SecondaryIndex = swap_index;
	}
	// クリティカルセクションアンロック
	CriticalSectionForScreen.Unlock();

	return;
}

// 画面描画処理
Void SNGraphics::DrawScreen(Handle hdc, Int32 width, Int32 height)
{
	UInt8 draw_align = SNConfiguration::SystemConfiguration.DrawAlign;
	Int32 config_width = SNConfiguration::SystemConfiguration.ScreenWidth;
	Int32 config_height = SNConfiguration::SystemConfiguration.ScreenHeight;

	// 画面サイズの変化を検知した場合は再計算
	if ((ScreenRect.Width != width) || (ScreenRect.Height != height))
	{
		// 画面サイズを記憶
		ScreenRect.Width = width;
		ScreenRect.Height = height;

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

		// DrawRectの更新はプライマリスレッドで行われ
		// 参照はアプリケーションスレッドから実行されるので
		// DrawRectのアクセス競合の懸念があるため排他制御する
		// ロック
		CriticalSectionForDrawRect.Lock();
		{
			// 描画範囲を更新
			DrawRect.PointX = offset_x;
			DrawRect.PointY = offset_y;
			DrawRect.Width = new_width;
			DrawRect.Height = new_height;
		}
		// アンロック
		CriticalSectionForDrawRect.Unlock();

		// 画面全体を黒塗りする
		::PatBlt((HDC)hdc, 0, 0, ScreenRect.Width, ScreenRect.Height, BLACKNESS);
	}

	// 描画処理はプライマリスレッドで実行され、
	// サーフェスフリップはアプリケーションスレッドで実行するため
	// サーフェスへのアクセス競合が発生する懸念があるため排他制御を行う
	// クリティカルセクションのロック
	CriticalSectionForScreen.Lock();
	{
		// 画面描画
		StretchBlt(
			(HDC)hdc,
			DrawRect.PointX,
			DrawRect.PointY,
			DrawRect.Width,
			DrawRect.Height,
			(HDC)ScreenSurface[PrimaryIndex].GetDC(),
			0,
			0,
			ScreenSurface[PrimaryIndex].GetWidth(),
			ScreenSurface[PrimaryIndex].GetHeight(),
			SRCCOPY);
	}
	// クリティカルセクションのアンロック
	CriticalSectionForScreen.Unlock();

	return;
}

// 画面座標系→サーフェス座標に変換
Boolean SNGraphics::ClientToSurface(SNPoint* point)
{
	SNRect draw_rect;
	Boolean clipping = false;

	// DrawRectの更新はプライマリスレッドで行われ
	// 参照はアプリケーションスレッドから実行されるので
	// DrawRectのアクセス競合の懸念があるため排他制御する
	// ロック
	CriticalSectionForDrawRect.Lock();
	{
		// 描画範囲を参照用にコピー
		draw_rect = DrawRect;
	}
	// アンロック
	CriticalSectionForDrawRect.Unlock();

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
	point->X = ((point->X - draw_rect.PointX) * SNConfiguration::SystemConfiguration.ScreenWidth) / draw_rect.Width;
	point->Y = ((point->Y - draw_rect.PointY) * SNConfiguration::SystemConfiguration.ScreenHeight) / draw_rect.Height;

	return clipping;
}
