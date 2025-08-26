#pragma once
#include "../../Include/SNFramework.h"
#include "SNLayer.h"

// システムレイヤクラス
class SNSystemLayer : public virtual SNLayer
{
public:
	// コンストラクタ
	SNSystemLayer();

	// デストラクタ
	virtual ~SNSystemLayer();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

private:

};

