#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUISystemText.h"
#include "SNList.h"

// テキスト
class SNGUIText : public virtual SNGUISystemText
{
public:
	// コンストラクタ
	SNGUIText();

	// デストラクタ
	virtual ~SNGUIText();

	// 色
	SNColor Color;
	SNListContainer* WorkSurface;

protected:
	virtual Void OnEntry();

	virtual Void OnExit();

	// 描画前処理
	virtual Void OnPreDraw();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);
};

