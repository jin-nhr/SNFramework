#pragma once
#include "SNFrameworkInternal.h"

// システムコンフィグ
// SNConfigの状態によらずいつでもアクセス可能とする
class SNSystemConfig
{
public:
	// フレームワークバージョン
	static constexpr UInt32 FrameworkVersion = 0x00000001;

	// アプリケーションID
	static constexpr String ApplicationID = (const String)L"9c8e9666-fd5f-841a-932e-b2635956048e";

	// アプリケーション名
	static constexpr String ApplicationName = (const String)L"TestApplication";

	// アプリケーションバージョン
	static constexpr UInt32 ApplicationVersion = 0x00000001;

	// ユーザーコンフィグファイル名
	static constexpr String ConfigFileName = (const String)L"UserConfiguration.cfg";

	// フォントファイル名
	static constexpr String FontFileName = (const String)L"Font.fnt";

	// デバッグ機能有効
	static constexpr Boolean DebugEnable = true;

	// シングルアプリ(false=二重起動防止)
	static constexpr Boolean SingleApp = true;

	// コンフィグファイル識別子
	static constexpr Char UserConfigIdentifier[4] = { L'S', L'N', L'C', L'F' };

	// コンフィグファイルバージョン
	static constexpr UInt32 UserConfigVersion = 0x00000001;

	// 画面幅
	static constexpr UInt32 ScreenWidth = 960;

	// 画面高さ
	static constexpr UInt32 ScreenHeight = 540;

	// フレームレート(FPS)
	static constexpr UInt32 FPS = 60;

	// BLTモード
	static constexpr UInt8 BltMode = SNBltMode::SNBltModeNearest;

	// 描画最小単位
	static constexpr UInt32 DrawAlign = 4;

	// 長押し判定時間(ms)
	static constexpr UInt32 KeyLongPressTime = 300;

	// キーリピート時間(ms)
	static constexpr UInt32 KeyRepeatTime = 100;

	// フリック移動量係数 (1/256)
	static constexpr UInt32 FlickTraceGain = 218;

	// フリック滑り量係数 (1/256)
	static constexpr UInt32 FlickFadeGain = 234;

	// ストレージへの最大同時要求数
	static constexpr UInt32 StorageRequestMax = 255;

	// ストップウォッチ履歴数
	static constexpr UInt32 StopWatchHistory = 60;

	// GUIテキストラベル初期サイズ
	static constexpr UInt32 GUITextLabelLength = 128;

};
