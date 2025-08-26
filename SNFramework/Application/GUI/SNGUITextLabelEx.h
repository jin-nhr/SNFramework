#pragma once
#include "../../Include/SNFramework.h"
#include "SNGUITextLabel.h"

// テキストラベル
class SNGUITextLabelEx : public virtual SNGUITextLabel
{
public:
	// コンストラクタ
	SNGUITextLabelEx();

	// デストラクタ
	virtual ~SNGUITextLabelEx();

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

	// 値設定
	virtual Void SetValue(Int64 value);

protected:
	SNMemory Format;	// フォーマット文字列
	Int64    Value;		// 設定値
	Boolean  Update;	// 更新フラグ
};
