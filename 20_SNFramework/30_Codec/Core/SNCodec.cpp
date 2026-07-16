#include "SNCodec.h"
#include "SNImageCodec.h"

// 初期化処理
Void SNCodec::Initialize()
{
	return;
}

// 起動準備
Void SNCodec::Startup()
{
	SNImageCodec::Initialize();

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
	SNImageCodec::Terminate();

	return;
}

// 終了
Void SNCodec::Terminate()
{


	return;
}
