#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNCriticalSection.h"
#include "SNSurface.h"

class SNSurface;

// グラフィクスクラス
class SNGraphics
{
// 共通メソッド/データ
public:
	// インスタンス生成/取得
	static SNGraphics* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	// 自身のインスタンス
	static SNGraphics* Me;


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNGraphics();
	
	// 初期化処理
	Void Initialize();
	
	// 起動準備
	Void Startup();
	
	// 実行
	// リターン：終了コード
	Int32 Run();

	// 終了前処理
	Void BeforeTerminate();
	
	// 終了
	Void Terminate();

	// サーフェス取得
	SNSurface* GetSurface();

	// サーフェスフリップ
	Void FlipSurface();

	// 画面描画処理
	Void DrawScreen(Handle hdc, Int32 width, Int32 height);


private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNGraphics();

	// 画面バッファ用クリティカルセクション
	SNCriticalSection CriticalSectionForScreen;

	// 画面サーフェス
	SNSurface* ScreenSurface[2];

	// プライマリサーフェスインデックス
	Int32 PrimaryIndex;

	// セカンダリサーフェスインデックス
	Int32 SecondaryIndex;

	// 描画対象画面サイズ
	SNRect ScreenRect;

	// 描画矩形データ
	SNRect DrawRect;
};

