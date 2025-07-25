#pragma once
#include "../Include/SNFramework.h"

// 状態クラス
class SNState
{
public:
	// 遷移先コード
	// -1:遷移なし
	// 0~:状態クラス毎に規程する
	enum TransitionCode
	{
		NoTransition = -1,
		Code1 = 0,
		Code2 = 1,
		Code3 = 2,
		Code4 = 3,
		MaxNum = 4,
	};

	// コンストラクタ
	SNState();

	// デストラクタ
	virtual ~SNState();

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
	virtual TransitionCode Step();

private:

};

