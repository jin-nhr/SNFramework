#pragma once
#include "../Include/SNFramework.h"


// システムコンフィギュレーション
// 4バイトアライメントにする
struct SNSystemConfiguration
{
	Char ApplicationID[SNApplicationIDLength];		// アプリケーションID
	Char ApplicationName[SNApplicationNameLength];	// アプリケーション名
	Boolean DualBootPermission;	// 二重起動許可
	UInt8 FPS;					// FPS
	UInt8 SurfaceType;			// サーフェスタイプ
	Int8 Reserve;				// リザーブ
	UInt32 ScreenWidth;			// 画面幅
	UInt32 ScreenHeight;		// 画面高さ
};

// ユーザーコンフィギュレーション
// 4バイトアライメントにする
// 4バイト単位ですべて足したときに0になるようにCheckSumを設定する
struct SNUserConfiguration
{
	UInt32 Reserve;		// リザーブ
	UInt32 CheckSum;	// チェックサム
};

// コンフィギュレーションデータ
struct SNConfigurationData
{
	SNSystemConfiguration System;	// システムコンフィギュレーション
	SNUserConfiguration User;		// ユーザーコンフィギュレーション
};

// コンフィギュレーションクラス
class SNConfiguration
{
// 共有メソッド/データ
public:
	// インスタンス生成/取得
	// リターン：インスタンス
	static SNConfiguration* GetInstance();

	// インスタンス破棄
	static Void Destroy();

private:
	static SNConfiguration* Me;	// 自身のインスタンス


// インスタンスメソッド/データ
public:
	// デストラクタ
	~SNConfiguration();

	// 初期化
	Void Initialize();

	// 起動準備
	Void Startup();

	// 実行
	Void Run();

	// 終了前処理
	Void BeforeTerminate();

	// 終了
	Void Terminate();


	SNConfigurationData ConfigurationData;	// コンフィギュレーションデータ

private:
	// コンストラクタ
	SNConfiguration();
};
