#include "SNUserAppBase.h"

// ユーザーアプリケーションベースクラス

// ユーザーアプリケーション
SNScene* SNUserAppBase::UserApplication = nullptr;

// コンストラクタ
SNUserAppBase::SNUserAppBase()
{
	// 自身を登録
	UserApplication = this;
	return;
}

// デストラクタ
SNUserAppBase::~SNUserAppBase()
{
	return;
}
