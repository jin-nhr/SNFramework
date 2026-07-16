#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUIBMTextLabel.h"
#include "SNGUIRect.h"
#include "SNLayerController.h"

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
	virtual Void OnDraw(SNSurface* grc);

private:
	SNGUIBMTextLabel Message;		// メッセージ
	SNGUIRect BackGround;			// 背景
};

