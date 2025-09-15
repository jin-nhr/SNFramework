#pragma once
#include "../Include/SNFramework.h"
#include "Controller/SNScene.h"

// ユーザーアプリケーションベース
// 本クラスを継承してユーザーアプリケーションを実装する
class SNUserApplication : public virtual SNScene
{
// スタティックメソッド
public:
	// ユーザーアプリケーション
	static SNScene* UserApplication;


// インスタンスメソッド
public:
	// デストラクタ
	virtual ~SNUserApplication();

protected:
	// コンストラクタ
	SNUserApplication();

private:

};
