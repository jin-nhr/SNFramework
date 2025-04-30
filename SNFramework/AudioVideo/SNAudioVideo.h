#pragma once
#include "../Include/SNFramework.h"

// オーディオビデオクラス
class SNAudioVideo
{
// 共通メソッド/データ
public:
	// インスタンス生成/取得
	static SNAudioVideo* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	// 自身のインスタンス
	static SNAudioVideo* Me;


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNAudioVideo();
	
	// 初期化処理
	// パラメータ：WinMainのパラメータを渡す
	Void Initialize();
	
	// 起動準備
	Void Startup();
	
	// 実行
	// リターン：終了コード
	Int32 Run();

	// 終了前処理
	Void BeforeTerminate();
	
	// 終了
	Void Terminate();

private:
	// コンストラクタ
	// 外部からのインスタンス生成は禁止
	SNAudioVideo();
};

