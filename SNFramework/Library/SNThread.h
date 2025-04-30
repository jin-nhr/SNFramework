#pragma once
#include "../Include/SNFramework.h"


// スレッドクラス
class SNThread
{
public:
	// コンストラクタ
	SNThread();

	// デストラクタ
	virtual ~SNThread();

	// スレッド生成/実行
	Void Run();

	// 実行状態の取得
	Boolean GetRunStatus();

	// スレッドハンドル取得
	Handle GetThreadHandle();

	// スレッドメイン処理
	Void ThreadMain();

private:
	// ユーザー実行関数(継承先で実装する)
	virtual Void UserMain();

	Handle ThreadHandle;		// スレッドハンドル
	Boolean RunStatus;			// 実行状態
};
