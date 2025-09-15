#pragma once
#include "../Include/SNFramework.h"
#include "../Storage/SNFile.h"


// システムコンフィギュレーション
struct SNSystemConfiguration
{
	String ApplicationID;		// アプリケーションID
	String ApplicationName;		// アプリケーション名
	String ConfigurationFileName;	// コンフィギュレーションファイル名

	Boolean DualBootPermission;	// 二重起動防止
	UInt8 FPS;					// FPS
	UInt8 FrameSkip;			// 最大フレームスキップ
	UInt8 DrawAlign;			// 描画境界

	UInt8 BltMode;				// BLTモード
	UInt8 Reserve[3];
	
	UInt32 StrageRequestMax;	// ストレージ要求最大数
	UInt32 ScreenWidth;			// 画面幅
	UInt32 ScreenHeight;		// 画面高さ
	UInt32 GUITextLabelLength;	// GUIテキストラベル長さ
	UInt32 SleepTimeThreshold;	// Sleep時間閾値
	UInt32 FPSMeasureTime;		// FPS測定時間(秒)
	UInt32 StopWatchHistory;	// ストップウォッチ履歴(フレーム数)
	UInt32 KeyLongPressTime;	// 長押し判定時間(ms)
	UInt32 KeyRepeatTime;		// キーリピート時間(ms)
	UInt32 FrickTraceGain;		// フリック移動量係数 (1/256)
	UInt32 FrickFadeGain;		// フリック滑り量係数 (1/256)
	Boolean KeySupportFlag[SNKeyCodeNum];		// キーサポート有無
};

// ユーザーコンフィギュレーション
// ユーザーによる設定変更が可能な項目
// 4バイトアライメントにする
// 4バイト単位ですべて足したときに0になるようにCheckSumを設定する
struct SNUserConfiguration
{
	Char Identifier[4];			// 識別子
	SNInputMapping InputMapping[SNInputDeviceNum];	// 入力マッピング
	UInt32 Reserve;				// リザーブ
	UInt32 CheckSum;			// チェックサム
};

// コンフィギュレーションクラス
class SNConfiguration
{
public:
	// 初期化
	static Void Initialize();

	// 起動準備
	static Void Startup();

	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();

	// 終了
	static Void Terminate();

	// チェックサム計算 (sizeは4バイト単位で指定)
	static UInt32 CalcCheckSum(Void* start, UInt32 size);

	// システムコンフィギュレーション
	static const SNSystemConfiguration SystemConfiguration;

	// ユーザーコンフィギュレーション/ 初期値
	static SNUserConfiguration UserConfiguration;
	static const SNUserConfiguration UserConfigurationInititalValue;

	// ファイル
	static SNFile ConfigFile;
};
