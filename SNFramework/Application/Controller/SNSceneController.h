#pragma once
#include "../../Include/SNFramework.h"
#include "SNScene.h"
#include "../../Library/SNList.h"


// シーン制御クラス
class SNSceneController : public virtual SNScene
{
public:
	// コンストラクタ
	SNSceneController();

	// デストラクタ
	virtual ~SNSceneController();

	// シーン数設定(登録できる数を決める)
	virtual Void SetSceneNum(UInt32 scene_num);

	// シーン登録
	virtual Void SetScene(SNScene* scene);



	// ローカル座標/サイズ設定
	virtual Void SetRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// ローカル座標/サイズ取得
	virtual SNRect GetRect();

	// 親座標/サイズ設定(グローバル)
	virtual Void SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// 親座標/サイズ取得(グローバル)
	virtual SNRect GetParentRect();

	// 移動
	// パラメータ：移動量を指定する
	virtual Void Move(Int32 x, Int32 y);

	// グローバル座標/サイズ計算
	virtual SNRect CalcGlobalRect();

	// 子情報更新
	virtual Void UpdateChildRect();

public:
	// シーンリスト
	SNList SceneList;

};
