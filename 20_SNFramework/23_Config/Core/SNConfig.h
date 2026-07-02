#pragma once
#include "SNFrameworkInternal.h"
#include "SNSystemConfig.h"
#include "SNUserConfig.h"
#include "SNFile.h"

// コンフィグクラス
// システム全体の設定管理を行う
class SNConfig
{
public:
	// 初期化
	static Void Initialize();

	// 起動準備
	static Void Startup();

	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();

	// 終了
	static Void Terminate();
};
