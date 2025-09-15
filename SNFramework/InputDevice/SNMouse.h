#pragma once
#include "../Include/SNFramework.h"


// マウス状態
struct SNMouseState
{
	SNPoint	Position;						// サーフェス上の論理座標
	Boolean PosClipping;					// 座標クリップ
	Boolean	ButtonState[SNMouseButtonNum];	// マウスボタン状態
};

// マウスクラス
class SNMouse
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 更新処理
	static Void Update();

	// 画面座標系→サーフェス座標変換
	// リターン：座標クリッピング有無
	static Boolean ScreenToSurface(SNPoint* point);

	// マウス状態
	static SNMouseState MouseState;

private:
	// ボタンコード配列
	static Int32 ButtonCode[SNMouseButtonNum];
};
