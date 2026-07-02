#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUIBMTextLabel.h"
#include "SNString.h"

// テキストラベル
class SNGUIBMTextLabelEx : public virtual SNGUIBMTextLabel
{
public:
	// コンストラクタ
	SNGUIBMTextLabelEx();

	// デストラクタ
	virtual ~SNGUIBMTextLabelEx();

	// テキスト設定
	virtual Void SetText(String text);

	// 値設定
	virtual Void SetValue(Int64 value);

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画処理
	virtual Void OnDraw(SNSurface* surface);

	SNString Format;	// フォーマット文字列
	Int64    Value;		// 設定値
	Boolean  Update;	// 更新フラグ
};
