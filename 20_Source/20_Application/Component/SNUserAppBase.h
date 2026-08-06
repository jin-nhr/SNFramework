#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// ユーザーアプリケーションベース
// 本クラスを継承してユーザーアプリケーションを実装する
class SNUserAppBase : public virtual SNScene
{
// スタティックメソッド
public:
	// ユーザーアプリケーション
	static SNScene* UserApplication;


// インスタンスメソッド
public:
	// デストラクタ
	virtual ~SNUserAppBase();

protected:
	// コンストラクタ
	SNUserAppBase();

private:

};
