#include "SNUserApplication.h"

// ユーザーアプリケーションベースクラス

// ユーザーアプリケーション
SNState* SNUserApplication::UserApplication = nullptr;

// コンストラクタ
SNUserApplication::SNUserApplication()
{
	// 自身を登録
	UserApplication = this;
	return;
}

// デストラクタ
SNUserApplication::~SNUserApplication()
{
	return;
}
