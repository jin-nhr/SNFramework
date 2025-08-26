#pragma once
#include "../../Include/SNFramework.h"
#include "SNLayer.h"


// レイヤ管理クラス
class SNLayerManager : public virtual SNLayer
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
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

	// レイヤ情報設定 (遷移先情報を設定する)
	Void SetLayerInfo(UInt8 layer_num, SNLayer** layer_list);

private:
	// レイヤ数
	UInt8 LayerNum;

	// レイヤリスト
	SNLayer** LayerList;
};
