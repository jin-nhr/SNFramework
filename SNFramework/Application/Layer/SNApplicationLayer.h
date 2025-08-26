#pragma once
#include "../../Include/SNFramework.h"
#include "SNLayer.h"
#include "../GUI/SNGUITextLabel.h"
#include "../GUI/SNGUIMonoRect.h"
#include "../GUI/SNGUIContainer.h"

// アプリケーションレイヤクラス
class SNApplicationLayer : public virtual SNLayer
{
public:
	static const String NoAppRegMessage;	// アプリ未登録メッセージ
	static const UInt32 GUIPartsNum = 2;	// GUI部品数

public:
	// コンストラクタ
	SNApplicationLayer();

	// デストラクタ
	virtual ~SNApplicationLayer();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

private:
	SNGUITextLabel NoAppRegistText;			// 未登録メッセージ
	SNGUIMonoRect BackGround;				// 背景
	SNGUIContainer GUIContainer;			// GUIコンテナ
	SNGUIBase* GUIPartsList[GUIPartsNum];	// GUI部品リスト
};

