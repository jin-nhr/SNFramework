#pragma once
#include "../../Include/SNFramework.h"
#include "../../Graphics/SNSurface.h"
#include "../../Application/SNEvent.h"


// シーンクラス
class SNScene
{
public:
	// コンストラクタ
	SNScene();

	// デストラクタ
	virtual ~SNScene();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行(インターフェース)
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理(インターフェース)
	virtual Void Draw(SNSurface* surface);



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



	
	Boolean Visible;		// 表示状態
	SNPoint ParentPosition;	// 親座標(グローバル)
	SNSize  ParentSize;		// 親サイズ
	SNPoint LocalPosition;	// 自身の座標(ローカル)
	SNSize  Size;			// 自身のサイズ

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
};
