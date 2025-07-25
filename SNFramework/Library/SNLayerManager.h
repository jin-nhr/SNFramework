#pragma once
#include "../Include/SNFramework.h"
#include "SNState.h"


// レイヤ管理クラス
class SNLayerManager : public virtual SNState
{
public:
	// コンストラクタ
	SNLayerManager();

	// デストラクタ
	virtual ~SNLayerManager();

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
	//           0~:状態クラス毎に規程する遷移先コード
	virtual SNState::TransitionCode Step();

protected:
	// レイヤ情報設定 (コンストラクタで派生元から状態、遷移先情報を設定する)
	Void SetLayerInfo(UInt8 layer_num, SNState** layer_list);

private:
	// レイヤ数
	UInt8 LayerNum;

	// レイヤリスト
	SNState** LayerList;
};
