#pragma once
#include "../../Include/SNFramework.h"
#include "../../Graphics/SNSurface.h"
#include "../State/SNState.h"
#include "../SNEvent.h"

// 状態クラス
class SNGUIBase : public virtual SNState
{
public:
	// コンストラクタ
	SNGUIBase();

	// デストラクタ
	virtual ~SNGUIBase();

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

	// 座標設定
	virtual Void SetPosition(Int32 x, Int32 y);

	// サイズ設定
	virtual Void SetSize(Int32 w, Int32 h);

	// 親座標、サイズ設定
	virtual Void SetRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// 親座標設定
	virtual Void SetParentPosition(Int32 x, Int32 y);

	// 親サイズ設定
	virtual Void SetParentSize(Int32 w, Int32 h);

	// 親座標、サイズ設定
	virtual Void SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// 親、自身の座標、サイズ設定
	// パラメータ：親、自身の座標、サイズを指定する
	virtual Void SetAllRect(Int32 px, Int32 py, Int32 pw, Int32 ph, Int32 x, Int32 y, Int32 w, Int32 h);

	// 移動
	// パラメータ：移動量を指定する
	virtual Void Move(Int32 x, Int32 y);

	// センタリング
	// パラメータ：横方向 or 縦方向への移動指定
	virtual Void Centering(Boolean horizontal, Boolean vertical);

	// グローバル座標/サイズ取得
	virtual SNRect GetGlobalRect();

	// オフセット座標/サイズ取得
	virtual SNRect GetOffsetRect();

protected:
	SNPoint GlobalParentPosition;	// 親のグローバル座標
	SNSize  ParentSize;				// 親のサイズ
	SNPoint LocalPosition;			// 自身の座標
	SNSize  Size;					// 自身のサイズ
};

