#pragma once
#include "SNFrameworkInternal.h"

// 仮想ダイレクトゲームパッド
// 実デバイス(GamePad)の入力情報をダイレクトにアプリケーションに提供する
class SNVirtualDirectGamePad
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 更新処理
	static Void Update();

	// 有効化
	static Void Activate(Boolean active);

	// 入力状態
	// 参照方法はSNGamePad準拠
	static Boolean* State[SNGamePadIDNum];

	static Boolean Active;		// 有効化フラグ
private:
};
