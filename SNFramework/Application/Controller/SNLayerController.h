#pragma once
#include "../../Include/SNFramework.h"
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
	// リターン：遷移先コード
	//           -1:遷移なし
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

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

	// 描画処理(実装用)
	virtual Void OnDraw(SNSurface* surface);
};
