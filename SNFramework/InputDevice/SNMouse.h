#pragma once
#include "../Include/SNFramework.h"


// マウス状態
struct SNMouseState
{
	SNPoint	Position;						// サーフェス上の論理座標
	Boolean	ButtonState[SNMouseButtonNum];	// マウスボタン状態
};

// マウスクラス
class SNMouse
{
public:
	// コンストラクタ
	SNMouse();

	// デストラクタ
	~SNMouse();

	// 初期化
	Void Initialize();

	// 終了
	Void Terminate();

	// 更新処理
	Void Update();

	// 状態取得
	const SNMouseState* GetState();

private:
	// マウス状態
	SNMouseState MouseState;

	// ボタンコード配列
	Int32 ButtonCode[SNMouseButtonNum];
};
