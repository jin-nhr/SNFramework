#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUIText.h"
#include "SNString.h"

// テキスト
class SNGUITextEx : public virtual SNGUIText
{
public:
	// コンストラクタ
	SNGUITextEx();

	// デストラクタ
	virtual ~SNGUITextEx();

	// テキスト設定
	virtual Void SetText(String text);

	// 値設定
	virtual Void SetValue(Int64 value);

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

	SNString Format;	// フォーマット文字列
	Int64    Value;		// 設定値
	Boolean  Update;	// 更新フラグ
};
