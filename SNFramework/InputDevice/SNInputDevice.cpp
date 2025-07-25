#include "SNInputDevice.h"

// 入力装置クラス


// 共通メソッド/データ

// インスタンス生成/取得
SNInputDevice* SNInputDevice::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNInputDevice;
	}

	return Me;
}

// インスタンス破棄
Void SNInputDevice::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNInputDevice* SNInputDevice::Me = nullptr;



// インスタンスメソッド/データ

// デストラクタ
SNInputDevice::~SNInputDevice()
{

	return;
}


// 初期化処理
Void SNInputDevice::Initialize()
{

	return;
}

// 起動準備
Void SNInputDevice::Startup()
{


	return;
}

// 実行
// リターン：終了コード
Int32 SNInputDevice::Run()
{


	return 0;
}

// 終了前処理
Void SNInputDevice::BeforeTerminate()
{


	return;
}

// 終了
Void SNInputDevice::Terminate()
{


	return;
}


// コンストラクタ
// 外部からのインスタンス生成は禁止
SNInputDevice::SNInputDevice()
{

	return;
}
