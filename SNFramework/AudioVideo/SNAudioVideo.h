#pragma once
#include "../Include/SNFramework.h"

// オーディオビデオクラス
class SNAudioVideo
{
public:
	// 初期化処理
	static Void Initialize();
	
	// 起動準備
	static Void Startup();
	
	// 実行
	// リターン：終了コード
	static Int32 Run();

	// 終了前処理
	static Void BeforeTerminate();
	
	// 終了
	static Void Terminate();
};

