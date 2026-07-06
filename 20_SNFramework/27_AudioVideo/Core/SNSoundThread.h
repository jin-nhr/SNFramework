#pragma once
#include "SNFrameworkInternal.h"

// オーディオビデオクラス
class SNAudioVideo
{
public:
	// 初期化処理
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

