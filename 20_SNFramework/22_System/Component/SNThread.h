#pragma once
#include "SNFrameworkInternal.h"


// スレッドクラス
// スレッドを生成し実行する
// スレッドが起動するとRunStatusがTrueになり
// UserMainが実行され、終了後RunStatusがFalseとなる
// 派生クラスにおいてUserMainを拡張することでユーザーの処理を実行可能とする
class SNThread
{
public:
	static Void EntryPoint(Void* l_parameter);

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

	// スレッド終了待ち
	// 同期処理のため注意
	Void WaitForThreadEnd();

private:
	// ユーザー実行関数(派生先で実装する)
	virtual Void UserMain();

	Handle ThreadHandle;		// スレッドハンドル
	volatile Boolean RunStatus;	// 実行状態
};
