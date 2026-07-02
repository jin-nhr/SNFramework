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


// グラフィクスクラス
// SNFrameworkにおける描画の制御を行う

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
SNRect SNGraphics::ScreenRect = { 0 };

// 描画矩形データ
SNRect SNGraphics::DrawRect = { 0 };


// 初期化処理
Void SNGraphics::Initialize()
{
	// クリティカルセクション初期化
	CriticalSectionForScreen.Initialize();
	CriticalSectionForDrawRect.Initialize();

	// GDI初期化
	SNGDI::Initialize();

	return;
}

// 起動準備
Void SNGraphics::Startup()
{
	Int32 width = SNSystemConfig::ScreenWidth;
	Int32 height = SNSystemConfig::ScreenHeight;

	// サーフェス生成
	ScreenSurface[0].CreateSurface(width, height);
	ScreenSurface[1].CreateSurface(width, height);

	// 描画領域を初期設定
	DrawRect.PointX = 0;
	DrawRect.PointY = 0;
	DrawRect.Width = width;
	DrawRect.Height = height;

	SNSystemColorTable::Initialize();

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

	// サーフェス解放
	ScreenSurface[0].DeleteSurface();
	ScreenSurface[1].DeleteSurface();

	return;
}

// 終了
Void SNGraphics::Terminate()
{
	// GDI終了
	SNGDI::Terminate();

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
	{
		SNAutoResource cs(&CriticalSectionForScreen);

		// サーフェスを入れ替え
		Int32 swap_index = PrimaryIndex;
		PrimaryIndex = SecondaryIndex;
		SecondaryIndex = swap_index;
	}

	return;
}

// 画面描画処理
Void SNGraphics::DrawScreen(Handle hdc, Int32 width, Int32 height)
{
	UInt8 draw_align = SNSystemConfig::DrawAlign;
	Int32 config_width = SNSystemConfig::ScreenWidth;
	Int32 config_height = SNSystemConfig::ScreenHeight;

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
		{
			SNAutoResource cs(&CriticalSectionForDrawRect);

			// 描画範囲を更新
			DrawRect.PointX = offset_x;
			DrawRect.PointY = offset_y;
			DrawRect.Width = new_width;
			DrawRect.Height = new_height;
		}

		{
			SNGDI gdi;
			// 画面全体を黒塗りする
			gdi.PatBlt(hdc, 0, 0, ScreenRect.Width, ScreenRect.Height, BLACKNESS);
		}
	}

	// 描画処理はプライマリスレッドで実行され、
	// サーフェスフリップはアプリケーションスレッドで実行するため
	// サーフェスへのアクセス競合が発生する懸念があるため排他制御を行う
	// クリティカルセクションのロック
	{
		SNAutoResource cs(&CriticalSectionForScreen);

		// 画面フリップ用のクリティカルセクション内で
		// 追加でGDIロックを行う
		// 他スレッドでは両方同時のロックを行わないこと
		{
			SNGDI gdi;

			// 画面描画
			gdi.StretchBlt(
				hdc,
				DrawRect.PointX,
				DrawRect.PointY,
				DrawRect.Width,
				DrawRect.Height,
				ScreenSurface[PrimaryIndex].GetDC(),
				0,
				0,
				ScreenSurface[PrimaryIndex].GetWidth(),
				ScreenSurface[PrimaryIndex].GetHeight(),
				SRCCOPY);
		}
	}

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
	{
		SNAutoResource cs(&CriticalSectionForDrawRect);
		// 描画範囲を参照用にコピー
		draw_rect = DrawRect;
	}

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
