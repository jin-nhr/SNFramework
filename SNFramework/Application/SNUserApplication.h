#pragma once
#include "../Include/SNFramework.h"
#include "State/SNState.h"

// ユーザーアプリケーションベース
// 本クラスを継承してユーザーアプリケーションを実装する
class SNUserApplication : public virtual SNState
{
// スタティックメソッド
public:
	// ユーザーアプリケーション
	static SNState* UserApplication;


// インスタンスメソッド
public:
	// デストラクタ
	virtual ~SNUserApplication();

protected:
	// コンストラクタ
	SNUserApplication();

private:

};
