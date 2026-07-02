#pragma once
#include "SNFrameworkInternal.h"
#include "SNSceneController.h"


// レイヤ制御クラス
class SNLayerController : public virtual SNSceneController
{
public:
	// コンストラクタ
	SNLayerController();

	// デストラクタ
	virtual ~SNLayerController();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行
	virtual Void Step();

	// 描画処理
	virtual Void Draw(SNSurface* surface);

protected:

};
