#include "SNSoftTimer.h"

// ソフトウェアタイマクラス

// フレームカウンタ
UInt32 SNSoftTimer::FrameCounter = 0;

// FPS
UInt32 SNSoftTimer::FPS = 0;


// 初期化
Void SNSoftTimer::Initialize(UInt32 fps)
{
	// 初期化
	FrameCounter = 0;

	// FPSセット
	FPS = fps;

	return;
}

// 終了
Void SNSoftTimer::Terminate()
{
	return;
}

// カウント
Void SNSoftTimer::Count()
{
	// フレームカウンタをカウントアップ
	FrameCounter++;

	return;
}

// コンストラクタ
SNSoftTimer::SNSoftTimer()
{
	// 変数初期化
	Enable = false;
	StartFrame = 0;
	WaitFrame = 0;

	return;
}

// デストラクタ
SNSoftTimer::~SNSoftTimer()
{
	return;
}

// タイマスタート
// パラメータ：待ち時間をミリ秒指定(最大約65秒)
Void SNSoftTimer::Start(UInt16 wait_ms)
{
	// 開始フレームにフレームカウンターをセット
	StartFrame = FrameCounter;

	// 待機フレーム数を計算(切り上げ処理)
	WaitFrame = (UInt32)((wait_ms * FPS + 999) / 1000);

	// タイマ有効化
	Enable = true;

	return;
}

// タイマリスタート
Void SNSoftTimer::Restart()
{
	// 開始フレームにフレームカウンターをセット
	StartFrame = FrameCounter;

	// タイマ有効化
	Enable = true;

	return;
}

// 停止
Void SNSoftTimer::Stop()
{
	// タイマ無効
	Enable = false;
	StartFrame = 0;
	WaitFrame = 0;

	return;
}

// タイムアウト確認
// リターン：true:タイムアウト
Boolean SNSoftTimer::IsTimeout()
{
	Boolean ret = false;

	// タイマ有効時のみ判定
	if (Enable)
	{
		// スタートフレームからの経過フレーム数が待機フレーム数を超えた？
		if (WaitFrame <= FrameCounter - StartFrame)
		{
			ret = true;
		}
	}

	return ret;
}

// 残フレーム数取得
UInt32 SNSoftTimer::ElapsedFrame()
{
	return FrameCounter - StartFrame;
}

// タイマ動作状態取得
Boolean SNSoftTimer::IsRunning()
{
	return Enable;
}
