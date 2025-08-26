#pragma once
#include "../Include/SNFramework.h"

// ソフトウェアタイマクラス
class SNSoftTimer
{
// スタティックメンバ
public:
	// 初期化
	static Void Initialize(UInt32 fps);

	// 終了
	static Void Terminate();

	// カウント
	static Void Count();

private:
	// フレームカウンタ
	static UInt32 FrameCounter;

	// FPS
	static UInt32 FPS;


// インスタンスメンバ
public:
	// コンストラクタ
	SNSoftTimer();

	// デストラクタ
	~SNSoftTimer();

	// タイマスタート
	// パラメータ：待ち時間をミリ秒指定(最大約65秒)
	Void Start(UInt16 wait_ms);

	// タイマリスタート
	Void Restart();

	// 停止
	Void Stop();

	// タイムアウト確認
	// リターン：true:タイムアウト
	Boolean IsTimeout();

	// 残フレーム数取得
	UInt32 ElapsedFrame();

	// タイマ動作状態取得
	Boolean IsRunning();

private:
	// 有効フラグ
	Boolean Enable;

	// 開始フレーム
	UInt32 StartFrame;

	// 待機フレーム
	UInt32 WaitFrame;
};
