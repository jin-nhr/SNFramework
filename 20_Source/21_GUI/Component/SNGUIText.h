#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNBMString.h"

// テキスト
class SNGUIText : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUIText();

	// デストラクタ
	virtual ~SNGUIText();

	// テキスト設定
	virtual Void SetText(String text);

	// テキスト設定
	virtual Void SetText(SNBMString* text);

	// 色
	SNColor Color;

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画前処理
	virtual Void OnPreDraw();

	// 描画処理
	virtual Void OnDraw();

	SNBMString Text;	// テキスト (自前で領域確保)
};

