#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundDevice.h"

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

	// 更新
	static Void Update();

	// 終了前処理
	static Void BeforeTerminate();

	// 終了
	static Void Terminate();

	// サウンドリソースロード
	static Void LoadSoundResource();

	// サウンドリソースロード完了判定
	static Boolean IsSoundResourceLoaded();

	// サウンドリソースアンロード
	static Void UnloadSoundResource();

};

