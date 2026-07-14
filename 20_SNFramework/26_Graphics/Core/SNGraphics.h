#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsContext.h"

// グラフィクスクラス
class SNGraphics
{
public:
	// 初期化処理
	static Void Initialize();

	// 起動準備
	// ウインドウ表示のためサーフェス準備までを実施
	// 他のシステムリソースはLoadSystemResourceで準備する
	static Void Startup();
	
	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// システムリソースロード
	static Void LoadSystemResource();

	// 更新
	static Void Update();

	// サーフェスフリップ
	static Void FlipSurface();

	// 描画範囲更新
	static Void UpdateDrawRect(SNRect* rect);

	// クライアント座標系→サーフェス座標に変換
	// 引数：画面座標を入力/ 処理後、サーフェス座標に更新する
	// リターン：座標のクリッピング有無
	static Boolean ClientToSurface(SNPoint* point);

	// フルスクリーン判定
	static Boolean IsFullScreen();

	// コンテキスト取得
	static SNGraphicsContext* GetContext();

	// コンテキスト解放
	static Void ReleaseContext();

private:
	// 前フレームフルスクリーン状態
	static Boolean PreFullScreenSts;

	// 前フレームウインドウサイズ
	static SNSize PreWindowSize;

	// 描画矩形データ
	static SNRect DrawRect;
};

