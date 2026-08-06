#pragma once
#include "SNFrameworkInternal.h"
#include "SNGUISystemText.h"
#include "SNString.h"

// 拡張システムテキスト
class SNGUISystemTextEx : public virtual SNGUISystemText
{
public:
	// コンストラクタ
	SNGUISystemTextEx();

	// デストラクタ
	virtual ~SNGUISystemTextEx();

	// テキスト設定
	virtual Void SetText(String text);

	// 値設定
	virtual Void SetValue(Int64 value);

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// 描画前処理
	virtual Void OnPreDraw();

	SNString Format;	// フォーマット文字列
	Int64    Value;		// 設定値
	Boolean  Update;	// 更新フラグ
};
