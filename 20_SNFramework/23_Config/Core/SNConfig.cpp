#include "SNConfig.h"
#include "SNUserConfig.h"
#include "SNWindowsAPI.h"

// コンフィグクラス

// 初期化
Void SNConfig::Initialize()
{
	return;
}

// 起動準備
Void SNConfig::Startup()
{
	// ユーザーコンフィグはファイルアクセスが必要なため
	// Startupで初期化する
	SNUserConfig::Initialize();

	return;
}

// 実行
Void SNConfig::Run()
{
	return;
}

// 終了前処理
Void SNConfig::BeforeTerminate()
{
	// ユーザーコンフィグはファイルアクセスが必要なため
	// BeforeTerminateで終了する
	SNUserConfig::Terminate();

	return;
}

// 終了
Void SNConfig::Terminate()
{
	return;
}

