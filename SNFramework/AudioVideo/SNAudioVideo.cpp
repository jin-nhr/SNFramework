#include "SNAudioVideo.h"

// オーディオビデオクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNAudioVideo* SNAudioVideo::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNAudioVideo;
	}

	return Me;
}

// インスタンス破棄
Void SNAudioVideo::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNAudioVideo* SNAudioVideo::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNAudioVideo::~SNAudioVideo()
{

	return;
}


// 初期化処理
Void SNAudioVideo::Initialize()
{

	return;
}

// 起動準備
Void SNAudioVideo::Startup()
{


	return;
}

// 実行
// リターン：終了コード
Int32 SNAudioVideo::Run()
{


	return 0;
}

// 終了前処理
Void SNAudioVideo::BeforeTerminate()
{


	return;
}

// 終了
Void SNAudioVideo::Terminate()
{


	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNAudioVideo::SNAudioVideo()
{

	return;
}
