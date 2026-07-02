#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// システムレイヤクラス
class SNSystemLayer : public virtual SNScene
{
public:
	// コンストラクタ
	SNSystemLayer();

	// デストラクタ
	virtual ~SNSystemLayer();

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

};

