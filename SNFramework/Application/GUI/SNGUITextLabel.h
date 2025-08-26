#pragma once
#include "../../Include/SNFramework.h"
#include "SNGUIBase.h"
#include "../../Library/SNMemory.h"

// テキストラベル
class SNGUITextLabel : public virtual SNGUIBase
{
public:
	// コンストラクタ
	SNGUITextLabel();

	// デストラクタ
	virtual ~SNGUITextLabel();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

	// テキスト設定
	virtual Void SetText(String text);

	// テキスト色設定
	virtual Void SetTextColor(UInt8 r, UInt8 g, UInt8 b);

	// 影有効設定
	virtual Void SetShadowEnable(Boolean shadow);

	// 影色設定
	virtual Void SetShadowColor(UInt8 r, UInt8 g, UInt8 b);

	// 影オフセット設定
	virtual Void SetShadowOffset(Int32 offset);

protected:
	SNMemory Text;			// テキスト (自前で領域確保)
	UInt32 TextColor;		// テキストカラー
	Boolean ShadowEnable;	// 影有効
	UInt32 ShadowColor;		// 影カラー
	UInt32 ShadowOffset;	// 影オフセット
};

