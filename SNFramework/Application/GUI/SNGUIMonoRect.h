#pragma once
#include "../../Include/SNFramework.h"
#include "SNGUIBase.h"

// 白黒矩形
class SNGUIMonoRect : public virtual SNGUIBase
{
public:
	// コンストラクタ
	SNGUIMonoRect();

	// デストラクタ
	virtual ~SNGUIMonoRect();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

	// 色設定 true:白 false:黒
	Void SetColor(Boolean whiteness);

protected:
	Boolean Whiteness;	// 白
};

