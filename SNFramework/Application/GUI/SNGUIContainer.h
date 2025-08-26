#pragma once
#include "../../Include/SNFramework.h"
#include "SNGUIBase.h"


// GUIコンテナクラス
class SNGUIContainer : public virtual SNGUIBase
{
public:
	// コンストラクタ
	SNGUIContainer();

	// デストラクタ
	virtual ~SNGUIContainer();

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
	//           -1:遷移なし
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

	// 座標設定
	virtual Void SetPosition(Int32 x, Int32 y);

	// サイズ設定
	virtual Void SetSize(Int32 w, Int32 h);

	// 親座標設定
	virtual Void SetParentPosition(Int32 x, Int32 y);

	// 親サイズ設定
	virtual Void SetParentSize(Int32 w, Int32 h);

	// 子情報更新
	virtual Void UpdateChildRect();

	// GUI部品設定 (遷移先情報を設定する)
	Void SetGUIPartsInfo(UInt8 gui_num, SNGUIBase** gui_list);

private:
	// GUI部品数
	UInt8 GUIPartsNum;

	// GUI部品リスト
	SNGUIBase** GUIPartsList;
};
