#pragma once
#include "../Include/SNFramework.h"

// キーボードクラス
class SNKeyboard
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 状態更新
	static Void Update();

	// キー状態
	static Boolean KeyState[SNKeyCodeNum];

	// チェック対象キーリスト
	static UInt8 CheckKeyList[SNKeyCodeNum];

	// チェック対象キー数
	static UInt8 CheckKeyNum;
};
