#pragma once
#include "SNFrameworkInternal.h"

// システムコンフィグ
// SNConfigの状態によらずいつでもアクセス可能とする
class SNSystemConfig
{
public:
	/////////////////////////////////////////////////////////////////
	// システム関連

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

	// デバッグ機能有効
	static constexpr Boolean DebugEnable = true;

	// シングルアプリ(false=二重起動防止)
	static constexpr Boolean SingleApp = true;

	// ストップウォッチ履歴数
	static constexpr UInt32 StopWatchHistory = 60;

	// スレッド終了タイムアウト(msec)
	static constexpr Int32 ThreadEndTimeout = 3000;


	/////////////////////////////////////////////////////////////////
	// ユーザーコンフィグ関連

	// コンフィグファイル識別子
	static constexpr Char UserConfigIdentifier[4] = { L'S', L'N', L'C', L'F' };

	// コンフィグファイルバージョン
	static constexpr UInt32 UserConfigVersion = 0x00000001;


	/////////////////////////////////////////////////////////////////
	// グラフィクス関連

	// フォントファイル名
	static constexpr String FontFileName = (const String)L"ビットマップフォント.png";

	// 画面幅
	static constexpr UInt32 ScreenWidth = 960;

	// 画面高さ
	static constexpr UInt32 ScreenHeight = 540;

	// フレームレート(FPS)
	static constexpr UInt32 FPS = 60;

	// フェード時間(ms)
	static constexpr UInt32 FadeTime = 500;

	// 連続フレームスキップ
	static constexpr UInt32 FrameSkip = 3;

	// 描画最小単位
	static constexpr UInt32 DrawAlign = 4;

	// ビットマップフォントのワークサーフェス数
	static constexpr Int32 BMFontWorkNum = 128;


	/////////////////////////////////////////////////////////////////
	// 入力デバイス関連

	// 長押し判定時間(ms)
	static constexpr UInt32 KeyLongPressTime = 300;

	// キーリピート時間(ms)
	static constexpr UInt32 KeyRepeatTime = 100;

	// フリック移動量係数 (1/256)
	static constexpr UInt32 FlickTraceGain = 218;

	// フリック滑り量係数 (1/256)
	static constexpr UInt32 FlickFadeGain = 234;


	/////////////////////////////////////////////////////////////////
	// ストレージ関連

	// ストレージへの最大同時要求数
	static constexpr UInt32 StorageRequestMax = 255;

	// ストレージタイムアウト時間(msec)
	static constexpr Int32 StorageTimeout = 3000;


	/////////////////////////////////////////////////////////////////
	// コーデック関連

	// コーデックへの最大同時要求数
	static constexpr UInt32 CodecRequestMax = 255;

	// コーデックタイムアウト時間(msec)
	static constexpr Int32 CodecTimeout = 1000;



	/////////////////////////////////////////////////////////////////
	// GUI関連

	// GUIブロックサイズ
	static constexpr Int32 GUIBlockSize = 16;

	// GUIテキストラベル初期サイズ
	static constexpr UInt32 GUITextLabelLength = 128;

	

};
