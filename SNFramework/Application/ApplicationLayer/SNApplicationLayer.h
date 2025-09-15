#pragma once
#include "../../Include/SNFramework.h"
#include "../Controller/SNStateController.h"
#include "SNApplicationLoading.h"
#include "SNApplicationNoUserApp.h"


// アプリケーションレイヤクラス
class SNApplicationLayer : public virtual SNStateController
{
public:
	// コンストラクタ
	SNApplicationLayer();

	// デストラクタ
	virtual ~SNApplicationLayer();

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode OnStep(SNEvent* event);

	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

private:
	SNApplicationLoading LoadingApp;	// ローディング
	SNApplicationNoUserApp NoUserApp;	// アプリ未登録
};

