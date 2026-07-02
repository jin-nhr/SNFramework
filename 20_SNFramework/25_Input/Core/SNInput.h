#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoftTimer.h"






// 入力装置クラス
// 物理入力デバイスを制御し、入力情報を仮想デバイスの入力イベントに変換する
// 物理デバイス
//   キーボード
//   マウス
//   ゲームパッド
//   スクリーンキーボード
//   スクリーンゲームパッド
// 仮想デバイス
//   仮想ポインティングデバイス
//   仮想ゲームパッド
//   仮想キーボード
//   仮想ダイレクトゲームパッド

class SNInput
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

	// 更新処理
	static Void Update();


private:
	// 状態更新
	// SNInput全体の動作状態を更新する
	static Void UpdateState();

	// デバイス更新
	// 実デバイスの状態を更新する
	static Void UpdateDevice();

	// 仮想デバイス更新
	// 実デバイスの入力をもとに仮想デバイスの情報を更新する
	static Void UpdateVirtualDevice();

	// アクティブ状態
	static Boolean Active;
};
