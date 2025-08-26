#include "SNGraphics.h"
#include "../System/SNWindowsAPI.h"
#include "../System/SNSystem.h"
#include "../Configuration/SNConfiguration.h"


// グラフィクスクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNGraphics* SNGraphics::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNGraphics;
	}

	return Me;
}

// インスタンス破棄
Void SNGraphics::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
		Me = nullptr;
	}

	return;
}

// 自身のインスタンス
SNGraphics* SNGraphics::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNGraphics::~SNGraphics()
{
	return;
}


// 初期化処理
Void SNGraphics::Initialize()
{
	// 子クラスのインスタンス生成
	CriticalSectionForScreen = new SNCriticalSection;
	CriticalSectionForDrawRect = new SNCriticalSection;

	// クリティカルセクション初期化
	CriticalSectionForScreen->Initialize();
	CriticalSectionForDrawRect->Initialize();

	return;
}

// 起動準備
Void SNGraphics::Startup()
{
	UInt8 surface_type = SNConfiguration::GetInstance()->ConfigurationData.System.SurfaceType;
	Int32 width = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenWidth;
	Int32 height = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenHeight;

	// サーフェス生成
	if (surface_type == SNSurfaceTypeDDB)
	{
		ScreenSurface[0] = new SNSurfaceDDB();
		ScreenSurface[1] = new SNSurfaceDDB();
	}
	else
	{
		ScreenSurface[0] = new SNSurfaceDIB();
		ScreenSurface[1] = new SNSurfaceDIB();
	}
	ScreenSurface[0]->CreateSurface(width, height);
	ScreenSurface[1]->CreateSurface(width, height);

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
	// サーフェス生成済みなら破棄
	if (ScreenSurface[0] != nullptr)
	{
		delete ScreenSurface[0];
		delete ScreenSurface[1];

		ScreenSurface[0] = nullptr;
		ScreenSurface[1] = nullptr;
	}

	// 子クラスの破棄
	delete CriticalSectionForScreen;
	delete CriticalSectionForDrawRect;

	return;
}

// サーフェス取得
SNSurface* SNGraphics::GetSurface()
{
	// セカンダリサーフェスを返す
	return ScreenSurface[SecondaryIndex];
}

// サーフェスフリップ
Void SNGraphics::FlipSurface()
{
	// 描画処理はプライマリスレッドで実行され、
	// サーフェスフリップはアプリケーションスレッドで実行するため
	// サーフェスへのアクセス競合が発生する懸念があるため排他制御を行う
	// クリティカルセクションロック
	CriticalSectionForScreen->Lock();
	{
		// サーフェスを入れ替え
		Int32 swap_index = PrimaryIndex;
		PrimaryIndex = SecondaryIndex;
		SecondaryIndex = swap_index;
	}
	// クリティカルセクションアンロック
	CriticalSectionForScreen->Unlock();

	return;
}

// 画面描画処理
Void SNGraphics::DrawScreen(Handle hdc, Int32 width, Int32 height)
{
	UInt8 draw_align = SNConfiguration::GetInstance()->ConfigurationData.System.DrawAlign;
	Int32 config_width = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenWidth;
	Int32 config_height = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenHeight;

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
		CriticalSectionForDrawRect->Lock();
		{
			// 描画範囲を更新
			DrawRect.PointX = offset_x;
			DrawRect.PointY = offset_y;
			DrawRect.Width = new_width;
			DrawRect.Height = new_height;
		}
		// アンロック
		CriticalSectionForDrawRect->Unlock();

		// 画面全体を黒塗りする
		::PatBlt((HDC)hdc, 0, 0, ScreenRect.Width, ScreenRect.Height, BLACKNESS);
	}

	// 描画処理はプライマリスレッドで実行され、
	// サーフェスフリップはアプリケーションスレッドで実行するため
	// サーフェスへのアクセス競合が発生する懸念があるため排他制御を行う
	// クリティカルセクションのロック
	CriticalSectionForScreen->Lock();
	{
		// 画面描画
		StretchBlt(
			(HDC)hdc,
			DrawRect.PointX,
			DrawRect.PointY,
			DrawRect.Width,
			DrawRect.Height,
			(HDC)ScreenSurface[PrimaryIndex]->GetDC()->GetDeviceContext(),
			0,
			0,
			ScreenSurface[PrimaryIndex]->GetWidth(),
			ScreenSurface[PrimaryIndex]->GetHeight(),
			SRCCOPY);
	}
	// クリティカルセクションのアンロック
	CriticalSectionForScreen->Unlock();

	return;
}

// 画面座標系→サーフェス座標に変換
Void SNGraphics::ScreenToSurface(SNPoint* point)
{
	POINT win_api_point;
	SNRect draw_rect;


	// DrawRectの更新はプライマリスレッドで行われ
	// 参照はアプリケーションスレッドから実行されるので
	// DrawRectのアクセス競合の懸念があるため排他制御する
	// ロック
	CriticalSectionForDrawRect->Lock();
	{
		// 描画範囲を参照用にコピー
		draw_rect = DrawRect;
	}
	// アンロック
	CriticalSectionForDrawRect->Unlock();

	// パラメータをWinAPI用の座標構造体にセット
	win_api_point.x = point->X;
	win_api_point.y = point->Y;

	// クライアント上の座標に変換
	ScreenToClient((HWND)SNSystem::GetInstance()->GetWindowDC(), &win_api_point);

	// X座標が描画左端より左なら左端でクリップ
	if (win_api_point.x < DrawRect.PointX)   win_api_point.x = DrawRect.PointX;
	// X座標が描画右端より右なら右端でクリップ
	if (DrawRect.PointX + DrawRect.Width <= win_api_point.x)  win_api_point.x = DrawRect.PointX + DrawRect.Width - 1;
	// Y座標が描画上端より上なら上でクリップ
	if (win_api_point.y < DrawRect.PointY)   win_api_point.y = DrawRect.PointY;
	// Y座標が描画下端より下なら下でクリップ
	if (DrawRect.PointY + DrawRect.Height <= win_api_point.y)  win_api_point.y = DrawRect.PointY + DrawRect.Height - 1;

	// 0割回避
	if (DrawRect.Width == 0) DrawRect.Width = 1;
	if (DrawRect.Height == 0) DrawRect.Height = 1;

	// 描画範囲でクリップした座標をサーフェス座標系にマッピング
	win_api_point.x = ((win_api_point.x - DrawRect.PointX) * SNConfiguration::GetInstance()->ConfigurationData.System.ScreenWidth) / DrawRect.Width;
	win_api_point.y = ((win_api_point.y - DrawRect.PointY) * SNConfiguration::GetInstance()->ConfigurationData.System.ScreenHeight) / DrawRect.Height;

	// パラメータの座標をサーフェス座標に更新
	point->X = win_api_point.x;
	point->Y = win_api_point.y;

	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNGraphics::SNGraphics()
{
	// 変数初期化
	DrawRect.PointX = 0;
	DrawRect.PointY = 0;
	DrawRect.Width = 0;
	DrawRect.Height = 0;

	ScreenRect.PointX = 0;
	ScreenRect.PointY = 0;
	ScreenRect.Width = 0;
	ScreenRect.Height = 0;

	PrimaryIndex = 0;
	SecondaryIndex = 1;

	ScreenSurface[0] = nullptr;
	ScreenSurface[1] = nullptr;

	return;
}
