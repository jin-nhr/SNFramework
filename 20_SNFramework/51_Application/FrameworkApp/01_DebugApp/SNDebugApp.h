#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNLayerController.h"
#include "SNGUISystemTextEx.h"

// デバッグアプリ
// 遷移コードなし

class SNDebugApp : public virtual SNScene
{
public:
	// コンストラクタ
	SNDebugApp();

	// デストラクタ
	virtual ~SNDebugApp();

protected:

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// フレーム処理
	virtual Void OnCycle();

	// 描画前処理
	virtual Void OnPreDraw();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	// 描画後処理
	virtual Void OnPostDraw();

private:
	SNLayerController cntSysInfo;		// システム情報コンテナ
	SNGUISystemText   txtSysInfoTitle;	// システム情報タイトル表示
	SNGUISystemTextEx txtFPS;			// PFS表示
	SNGUISystemTextEx txtProcFPS;		// 処理FPS
	SNGUISystemTextEx txtSkip;			// フレームスキップ
	SNGUISystemTextEx txtProcTime;		// 処理時間表示
};

