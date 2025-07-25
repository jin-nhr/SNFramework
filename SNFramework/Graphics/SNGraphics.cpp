#include "SNGraphics.h"
#include "../System/SNWindowsAPI.h"
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
	// クリティカルセクション初期化
	CriticalSectionForScreen.Initialize();

	return;
}

// 起動準備
Void SNGraphics::Startup()
{
	UInt8 surface_type = SNConfiguration::GetInstance()->ConfigurationData.System.SurfaceType;
	Int32 width = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenWidth;
	Int32 height = SNConfiguration::GetInstance()->ConfigurationData.System.ScreenHeight;


	// サーフェス生成
	if (surface_type == SurfaceTypeDDB)
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
	// クリティカルセクションロック
	CriticalSectionForScreen.Lock();

	// サーフェスを入れ替え
	Int32 swap_index = PrimaryIndex;
	PrimaryIndex = SecondaryIndex;
	SecondaryIndex = swap_index;

	// クリティカルセクションアンロック
	CriticalSectionForScreen.Unlock();

	return;
}

// 画面描画処理
Void SNGraphics::DrawScreen(Handle hdc, Int32 width, Int32 height)
{
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
		new_width = (new_width / 4) * 4;
		new_height = (new_height / 4) * 4;

		// 描画領域のオフセット計算（中央配置）
		Int32 offset_x = (width - new_width) / 2;
		Int32 offset_y = (height - new_height) / 2;

		DrawRect.PointX = offset_x;
		DrawRect.PointY = offset_y;
		DrawRect.Width = new_width;
		DrawRect.Height = new_height;

		// 画面全体を黒塗りする
		::PatBlt((HDC)hdc, 0, 0, ScreenRect.Width, ScreenRect.Height, BLACKNESS);
	}

	// クリティカルセクションのロック
	CriticalSectionForScreen.Lock();

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

	// クリティカルセクションのアンロック
	CriticalSectionForScreen.Unlock();

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
