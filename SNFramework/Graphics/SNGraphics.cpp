#include "SNGraphics.h"

// グラフィクスクラス


// 共通メソッド/データ

// インスタンス生成/取得
SNGraphics* SNGraphics::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNGraphics;
	}

	return Me;
}

// インスタンス破棄
Void SNGraphics::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNGraphics* SNGraphics::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNGraphics::~SNGraphics()
{

	return;
}


// 初期化処理
// パラメータ：WinMainのパラメータを渡す
Void SNGraphics::Initialize()
{

	return;
}

// 起動準備
Void SNGraphics::Startup()
{


	return;
}

// 実行
// リターン：終了コード
Int32 SNGraphics::Run()
{


	return 0;
}

// 終了前処理
Void SNGraphics::BeforeTerminate()
{


	return;
}

// 終了
Void SNGraphics::Terminate()
{


	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNGraphics::SNGraphics()
{

	return;
}
