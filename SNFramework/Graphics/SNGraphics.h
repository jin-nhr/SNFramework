#pragma once
#include "../Include/SNFramework.h"
#include "../Library/SNCriticalSection.h"
#include "SNSurfaceDDB.h"

class SNSurface;

// グラフィクスクラス
class SNGraphics
{
public:
	// 初期化処理
	static Void Initialize();
	
	// 起動準備
	static Void Startup();
	
	// 実行
	// リターン：終了コード
	static Int32 Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();

	// サーフェス取得
	static SNSurface* GetSurface();

	// サーフェスフリップ
	static Void FlipSurface();

	// 画面描画処理
	static Void DrawScreen(Handle hdc, Int32 width, Int32 height);

	// クライアント座標系→サーフェス座標に変換
	// 引数：画面座標を入力/ 処理後、サーフェス座標に更新する
	// リターン：座標のクリッピング有無
	static Boolean ClientToSurface(SNPoint* point);

private:
	// 画面バッファ用クリティカルセクション
	static SNCriticalSection CriticalSectionForScreen;

	// 描画矩形データ用クリティカルセクション
	static SNCriticalSection CriticalSectionForDrawRect;

	// 画面サーフェス
	static SNSurfaceDDB ScreenSurface[2];

	// プライマリサーフェスインデックス
	static Int32 PrimaryIndex;

	// セカンダリサーフェスインデックス
	static Int32 SecondaryIndex;

	// 描画対象画面サイズ
	static SNRect ScreenRect;

	// 描画矩形データ
	static SNRect DrawRect;
};

