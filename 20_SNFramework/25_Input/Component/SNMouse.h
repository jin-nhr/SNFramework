#pragma once
#include "SNFrameworkInternal.h"


// マウスクラス
// マウスの入力状態を監視する
// OSによるマウスボタンの左右入れ替えを加味した状態を保持する
// カーソル座標はアプリケーションの画面座標系に変換する

class SNMouse
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 更新処理
	static Void Update();


	// マウス状態
	static SNPoint Position;							// サーフェス上の論理座標
	static Boolean PosClipping;						// 座標クリップした？(マウスが画面外か？)
	static Boolean ButtonState[SNMouseButtonNum];	// マウスボタン状態

private:
	// 座標の更新
	static Void UpdatePos();

	// ボタン状態の更新
	static Void UpdateButtonState();


	static Boolean ButtonSwap;						// 左右ボタン入替

};
