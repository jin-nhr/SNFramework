#pragma once
#include "../../Include/SNFramework.h"
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
	// リターン：遷移先コード
	//           -1:遷移なし
	//           0~:状態クラス毎に規程する遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

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
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode OnStep(SNEvent* event);

	// 描画処理(実装用)
	virtual Void OnDraw(SNSurface* surface);

private:
	// 現在状態
	UInt8 NowState;

	// 遷移先情報リスト
	SNList TransitionList;
};
