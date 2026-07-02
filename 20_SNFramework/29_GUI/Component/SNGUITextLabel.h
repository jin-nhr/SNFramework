#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNString.h"

// テキストラベル
class SNGUITextLabel : public virtual SNScene
{
public:
	// コンストラクタ
	SNGUITextLabel();

	// デストラクタ
	virtual ~SNGUITextLabel();

	// テキスト設定
	virtual Void SetText(String text);

	// テキスト色設定
	virtual Void SetTextColor(SNColorCode color);

	// 影有効設定
	virtual Void SetShadowEnable(Boolean shadow);

	// 影色設定
	virtual Void SetShadowColor(SNColorCode color);

	// 影オフセット設定
	virtual Void SetShadowOffset(Int32 offset);

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	SNString Text;				// テキスト (自前で領域確保)
	SNColorCode TextColor;		// テキストカラー
	Boolean ShadowEnable;		// 影有効
	SNColorCode ShadowColor;	// 影カラー
	UInt32 ShadowOffset;		// 影オフセット
};
