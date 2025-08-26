#pragma once
#include "../Include/SNFramework.h"

// キーボードクラス
class SNKeyboard
{
	// 静的メンバ
public:

	// インスタンスメンバ
public:
	// コンストラクタ
	SNKeyboard();

	// デストラクタ
	~SNKeyboard();

	// 初期化
	Void Initialize();

	// 終了
	Void Terminate();

	// 状態更新
	Void Update();

	// 状態取得
	const Boolean* GetState();

	// 

private:
	// キー状態
	Boolean KeyState[SNKeyCodeNum];
};
