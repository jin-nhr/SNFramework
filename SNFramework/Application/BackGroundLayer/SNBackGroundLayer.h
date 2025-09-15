#pragma once
#include "../../Include/SNFramework.h"
#include "../Controller/SNScene.h"

// バックグラウンドレイヤクラス
class SNBackGroundLayer : public virtual SNScene
{
public:
	// コンストラクタ
	SNBackGroundLayer();

	// デストラクタ
	virtual ~SNBackGroundLayer();

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

