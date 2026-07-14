#pragma once
#include "SNFrameworkInternal.h"
#include "SNSceneController.h"


// 状態制御クラス
class SNStateController : public virtual SNSceneController
{
public:
	// コンストラクタ
	SNStateController();

	// デストラクタ
	virtual ~SNStateController();

	// シーン数設定(登録できる数を決める)
	virtual Void SetSceneNum(UInt32 scene_num);

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行
	virtual Void Step();

	// 描画処理
	virtual Void Draw(SNGraphicsContext* grc);

	// シーン設定
	// sceneにはサブ状態となるシーンを設定する
	// dist1～4はTransitionCode1～4に該当する遷移先Noを入れる
	// 遷移先No = SetSceneの登録順と一致する
	// つまり最初に登録したSceneが0, 次が1, その次が2 ... となる
	virtual Void SetScene(SNScene* scene, UInt8 dist1, UInt8 dist2, UInt8 dist3, UInt8 dist4);

	// シーン設定完了
	// シーンの登録がすべて完了した後で実行すること
	virtual Void CompleteSceneSet();

protected:


private:
	// 現在状態
	UInt8 NowState;

	// 遷移先情報リスト
	SNList TransitionList;
};
