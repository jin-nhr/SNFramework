#include "SNThread.h"
#include "SNWindowsAPI.h"
#include "SNSystemTimer.h"
#include "SNSystemConfig.h"

// スレッドクラス

// スレッドのエントリポイント
static Void EntryPoint(Void* l_parameter)
{
	// パラメータからスレッドクラスのインスタンスを取得
	SNThread* this_ptr = (SNThread*)l_parameter;

	// スレッドメイン実行
	this_ptr->ThreadMain();

	return;
}


// コンストラクタ
SNThread::SNThread()
{
	// 変数初期化
	ThreadHandle = nullptr;
	RunStatus = false;

	return;
}

// デストラクタ
SNThread::~SNThread()
{
	// 動作中でハンドル有効なら強制終了する
	if (RunStatus)
	{
		if (ThreadHandle != nullptr)
		{
			// ここに来ないように作る必要あり！
			TerminateThread((HANDLE)ThreadHandle, 0);
		}
	}

	return;
}

// スレッド生成/実行
Void SNThread::Run()
{
	// 停止しているときだけ処理
	if (!RunStatus)
	{
		// ハンドル無効のときだけスレッド生成
		if (ThreadHandle == nullptr)
		{
			ThreadHandle = (void*)_beginthread(EntryPoint, 0, this);

			// 生成と同時に勝手に実行される
		}
	}

	return;
}

// 実行状態の取得
Boolean SNThread::GetRunStatus()
{
	return RunStatus;
}

// スレッドハンドル取得
Handle SNThread::GetThreadHandle()
{
	return ThreadHandle;
}


// スレッドメイン処理
Void SNThread::ThreadMain()
{
	// 動作状態セット
	RunStatus = true;

	// ユーザー実行関数
	UserMain();

	// 停止状態セット
	RunStatus = false;

	return;
}

// ユーザー実行関数(継承先で実装する)
Void SNThread::UserMain()
{
	// 実装は継承先クラスで行うため無処理としておく
	return;
}

// スレッド終了待ち
// 同期処理のため注意
Void SNThread::WaitForThreadEnd()
{
	SNSystemTimer timer(SNSystemConfig::ThreadEndTimeout);
	timer.Start();

	while (GetRunStatus() && (!timer.CheckTimeout()))
	{
		::Sleep(1);
	}

	return;
}
