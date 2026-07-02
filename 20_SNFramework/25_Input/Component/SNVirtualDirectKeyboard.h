#pragma once
#include "SNFrameworkInternal.h"

// 仮想ダイレクトキーボード
// 実デバイス(Keyboard)の入力情報をダイレクトにアプリケーションに提供する
class SNVirtualDirectKeyboard
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
	// 参照方法はSNKeyboard準拠
	static Boolean* State;

	static Boolean Active;		// 有効化フラグ
private:
};
