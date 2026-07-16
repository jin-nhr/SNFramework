#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNLayerController.h"
#include "SNGUISystemTextEx.h"

// デバッグアプリ Run
// 遷移コードなし

class SNDebugAppRun : public virtual SNScene
{
public:
	// コンストラクタ
	SNDebugAppRun();

	// デストラクタ
	virtual ~SNDebugAppRun();

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

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

private:
	SNLayerController CntSysInfo;		// システム情報コンテナ
	SNGUISystemText LblSysInfoTitle;	// システム情報タイトル表示
	SNGUISystemTextEx LblFPS;			// PFS表示
	SNGUISystemTextEx LblSkip;			// フレームスキップ
	SNGUISystemTextEx LblProcTime;		// 処理時間表示
};

