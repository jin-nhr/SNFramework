#include "SNConfiguration.h"

// コンフィギュレーションクラス


// システムコンフィギュレーション設定値
static const SNSystemConfiguration SystemConfigurationInitialValue = {
	L"9c8e9666-fd5f-841a-932e-b2635956048e",	// アプリケーションID
	L"TestApplication",	// アプリケーション名
	false,			// 二重起動許可 true:許可 false:禁止
	60,				// FPS
	SurfaceTypeDDB,	// サーフェスタイプ
	0,				// リザーブ
	800,			// 画面幅
	600,			// 画面高さ
};

// ユーザーコンフィギュレーション初期値
static const SNUserConfiguration UserConfgurationInititalValue = {
	0,			// リザーブ
	0,			// チェックサム
};



// インスタンス生成/取得
// リターン：インスタンス
SNConfiguration* SNConfiguration::GetInstance()
{
	if (Me == nullptr)
	{
		Me = new SNConfiguration;
	}

	return Me;
}

// インスタンス破棄
Void SNConfiguration::Destroy()
{
	if (Me != nullptr)
	{
		delete Me;
	}

	Me = nullptr;

	return;
}

// 自身のインスタンス
SNConfiguration* SNConfiguration::Me = nullptr;

// デストラクタ
SNConfiguration::~SNConfiguration()
{
	return;
}

// 初期化
Void SNConfiguration::Initialize()
{
	return;
}

// 起動準備
Void SNConfiguration::Startup()
{
	return;
}

// 実行
Void SNConfiguration::Run()
{
	return;
}

// 終了前処理
Void SNConfiguration::BeforeTerminate()
{
	return;
}

// 終了
Void SNConfiguration::Terminate()
{
	return;
}

// コンストラクタ
SNConfiguration::SNConfiguration()
{
	// コンフィギュレーション初期値設定
	ConfigurationData.System = SystemConfigurationInitialValue;
	ConfigurationData.User = UserConfgurationInititalValue;

	return;
}

