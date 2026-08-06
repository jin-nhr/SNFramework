#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"

// アプリケーションスレッド
// アプリケーションスレッドのメイン関数の実装
// SNApplicationのUserMainを実行する

class SNApplicationThread : public SNThread
{
private:
	// スレッドクラスのユーザー実行関数
	Void UserMain();
};
