#include "SNCodec.h"
#include "SNImageCodec.h"
#include "SNSoundCodec.h"
#include "SNWindowsAPI.h"

// 初期化処理
Void SNCodec::Initialize()
{
	MFStartup(MF_VERSION);
	return;
}

// 起動準備
Void SNCodec::Startup()
{
	SNImageCodec::Initialize();
	SNSoundCodec::Initialize();

	return;
}

// 実行
Void SNCodec::Run()
{
	return;
}

// 終了前処理
Void SNCodec::BeforeTerminate()
{
	SNSoundCodec::Terminate();
	SNImageCodec::Terminate();

	return;
}

// 終了
Void SNCodec::Terminate()
{
	MFShutdown();
	return;
}
