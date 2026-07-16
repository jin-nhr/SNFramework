#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNBMString.h"

// システムテキスト
class SNGUISystemText : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUISystemText();

	// デストラクタ
	virtual ~SNGUISystemText();

	// テキスト設定
	virtual Void SetText(String text);

	// テキスト設定
	virtual Void SetText(SNBMString* text);

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	SNBMString Text;			// テキスト (自前で領域確保)
};

