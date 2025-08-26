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
	UInt8 FrameSkip;			// 最大フレームスキップ
	UInt8 SurfaceType;			// サーフェスタイプ
	UInt8 DrawAlign;			// 描画単位
	UInt8 Reserve[3];			// リザーブ
	UInt32 ScreenWidth;			// 画面幅
	UInt32 ScreenHeight;		// 画面高さ
	UInt32 GUITextLabelLength;	// GUIテキストラベル長さ
	UInt32 SleepTimeThreshold;	// Sleep時間閾値
	UInt32 FPSMeasureTime;		// FPS測定時間(秒)
	UInt32 StopWatchHistory;	// ストップウォッチ履歴(フレーム数)
	UInt32 FrickTraceGain;		// フリック移動量係数 (1/256)
	UInt32 FrickFadeGain;		// フリック滑り量係数 (1/256)
	Boolean KeySupportFlag[SNKeyCodeNum];							// キーサポート有無
};


// ユーザーコンフィギュレーション
// ユーザーによる設定変更が可能な項目
// 4バイトアライメントにする
// 4バイト単位ですべて足したときに0になるようにCheckSumを設定する
struct SNUserConfiguration
{
	SNInputMapping InputMapping[SNInputDeviceNum];	// 入力マッピング
	UInt32 KeyLongPressTime;	// 長押し判定時間
	UInt32 KeyRepeatTime;		// キーリピート時間
	UInt32 Reserve;		// リザーブ
	UInt32 CheckSum;	// チェックサム
};


// コンフィギュレーションデータ
struct SNConfigurationData
{
	SNSystemConfiguration System;		// システムコンフィギュレーション
	SNUserConfiguration User;			// ユーザーコンフィギュレーション
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
	// 自身のインスタンス
	static SNConfiguration* Me;


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

	// コンフィギュレーションデータ
	SNConfigurationData ConfigurationData;

private:
	SNConfiguration();
};
