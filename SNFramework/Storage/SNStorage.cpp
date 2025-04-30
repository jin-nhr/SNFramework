#include "SNStorage.h"

// ストレージクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNStorage* SNStorage::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNStorage;
	}

	return Me;
}

// インスタンス破棄
Void SNStorage::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNStorage* SNStorage::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNStorage::~SNStorage()
{

	return;
}


// 初期化処理
// パラメータ：WinMainのパラメータを渡す
Void SNStorage::Initialize()
{

	return;
}

// 起動準備
Void SNStorage::Startup()
{


	return;
}

// 実行
// リターン：終了コード
Int32 SNStorage::Run()
{


	return 0;
}

// 終了前処理
Void SNStorage::BeforeTerminate()
{


	return;
}

// 終了
Void SNStorage::Terminate()
{


	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNStorage::SNStorage()
{

	return;
}
