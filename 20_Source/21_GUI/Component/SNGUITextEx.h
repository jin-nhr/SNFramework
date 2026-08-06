#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUISystemTextEx.h"
#include "SNList.h"

// テキスト
class SNGUITextEx : public virtual SNGUISystemTextEx
{
public:
	// コンストラクタ
	SNGUITextEx();

	// デストラクタ
	virtual ~SNGUITextEx();

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

