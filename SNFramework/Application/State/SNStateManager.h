#pragma once
#include "../../Include/SNFramework.h"
#include "SNState.h"


// 遷移先情報
struct SNStateTransitionInfo
{
	UInt8 DestinationState[SNTransitionCodeNum];	// 子状態の遷移コード毎の遷移先を定義
	SNTransitionCode ReturnCode;					// 自身が親に返す遷移先コード
};

// 状態管理クラス
class SNStateManager : public virtual SNState
{
public:
	// コンストラクタ
	SNStateManager();

	// デストラクタ
	virtual ~SNStateManager();

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

	// 遷移情報設定 (遷移先情報を設定する)
	Void SetTransitionInfo(UInt8 state_num, SNState** state_list, SNStateTransitionInfo* transition_info);

private:
	// 現在状態
	UInt8 NowState;

	// 状態数
	UInt8 StateNum;

	// 状態リスト
	SNState** StateList;

	// 状態遷移テーブル
	SNStateTransitionInfo* TransitionInfo;
};
