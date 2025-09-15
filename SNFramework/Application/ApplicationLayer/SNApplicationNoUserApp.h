#pragma once
#include "../../Include/SNFramework.h"
#include "../Controller/SNScene.h"
#include "../GUI/SNGUITextLabel.h"
#include "../GUI/SNGUIMonoRect.h"
#include "../Controller/SNLayerController.h"

// アプリケーション(ユーザー登録)
class SNApplicationNoUserApp : public virtual SNLayerController
{
public:
	// コンストラクタ
	SNApplicationNoUserApp();

	// デストラクタ
	virtual ~SNApplicationNoUserApp();

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
	SNGUITextLabel Message;			// メッセージ
	SNGUIMonoRect BackGround;		// 背景
};

