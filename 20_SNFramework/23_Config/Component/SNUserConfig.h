#pragma once
#include "SNFrameworkInternal.h"
#include "SNSystemConfig.h"
#include "SNFile.h"

// ユーザーコンフィグデータ
// 4バイトアライメント
struct SNUserConfigData
{
	//--------------------------------------------
	Char Identifier[4];			// 識別子
	//--------------------------------------------
	UInt32 Version;				// バージョン
	//--------------------------------------------
	Boolean FullScreen;			// 最大化設定
	UInt8 Reserve[3];			// リザーブ
	//--------------------------------------------
	SNKeyCode	KeyboardMapping[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum];	// キーボード入力マッピング
	//--------------------------------------------
	SNGamePadID GamePadSelect[SNVirtualGamePadIDNum];	// ゲームパッド割り当て
	//--------------------------------------------
	SNGamePadButton	GamePadMapping[SNVirtualGamePadIDNum][SNVirtualGamePadButtonNum];	// ゲームパッド入力マッピング
	//--------------------------------------------
	UInt32 CheckSum;				// チェックサム
	//--------------------------------------------
};


// ユーザーコンフィグ
// ユーザーによる設定変更などを保持する
// SNConfigのStartup以降で参照可能となる
class SNUserConfig
{
public:
	// 初期化
	static Void Initialize();

	// 終了処理
	static Void Terminate();

	// ユーザーデータ
	static SNUserConfigData Data;


private:
	// チェックサム計算 (sizeは4バイト単位で指定)
	static UInt32 CalcCheckSum(Void* start, UInt32 size);

	// コンフィグファイル読み込み
	static Void LoadUserConfig();

	// コンフィグファイル保存
	static Void SaveUserConfig();

	// ファイル
	static SNFile ConfigFile;

	// ユーザーデータ初期値
	static constexpr SNUserConfigData InitValue = {
		//--------------------------------------------
		SNSystemConfig::UserConfigIdentifier[0],// 識別子=SNCF (1文字目)
		SNSystemConfig::UserConfigIdentifier[1],// 識別子=SNCF (2文字目)
		SNSystemConfig::UserConfigIdentifier[2],// 識別子=SNCF (3文字目)
		SNSystemConfig::UserConfigIdentifier[3],// 識別子=SNCF (4文字目)
		//--------------------------------------------
		SNSystemConfig::UserConfigVersion,	// バージョン
		//--------------------------------------------
		false,						// 最大化設定
		0, 0, 0,					// リザーブ
		//--------------------------------------------

		// キーボード入力マッピング
		// 仮想パッド1
		SNKeyCodeUp,				// 方向キー上
		SNKeyCodeDown,			    // 方向キー下
		SNKeyCodeLeft,				// 方向キー左
		SNKeyCodeRight,				// 方向キー右
		SNKeyCodeZ,					// 決定(Aボタン)
		SNKeyCodeX,					// キャンセル(Bボタン)
		SNKeyCodeV,					// メニュー(Xボタン)
		SNKeyCodeC,					// アクション(Yボタン)
		SNKeyCodeN,					// スタート
		SNKeyCodeB,					// セレクト
		SNKeyCodeA,					// ページ戻し(Lボタン)
		SNKeyCodeS,					// ページ送り(Rボタン)
		SNKeyCodeD,					// Lトリガ(L2ボタン)
		SNKeyCodeF,					// Rトリガ(R2ボタン)
		SNKeyCodeG,					// リストアップ(拡大)
		SNKeyCodeH,					// リストダウン(縮小)

		// 仮想パッド2
		SNKeyCodeNull,				// 方向キー上
		SNKeyCodeNull,			    // 方向キー下
		SNKeyCodeNull,				// 方向キー左
		SNKeyCodeNull,				// 方向キー右
		SNKeyCodeNull,				// 決定(Aボタン)
		SNKeyCodeNull,				// キャンセル(Bボタン)
		SNKeyCodeNull,				// メニュー(Xボタン)
		SNKeyCodeNull,				// アクション(Yボタン)
		SNKeyCodeNull,				// スタート
		SNKeyCodeNull,				// セレクト
		SNKeyCodeNull,				// ページ戻し(Lボタン)
		SNKeyCodeNull,				// ページ送り(Rボタン)
		SNKeyCodeNull,				// Lトリガ(L2ボタン)
		SNKeyCodeNull,				// Rトリガ(R2ボタン)
		SNKeyCodeNull,				// リストアップ(拡大)
		SNKeyCodeNull,				// リストダウン(縮小)
		
		//--------------------------------------------
		// パッド割り当て
		SNGamePadID1,				// 仮想パッド1への割り当て
		SNGamePadID2,				// 仮想パッド2への割り当て
		
		//--------------------------------------------
		// ゲームパッド入力マッピング
		// 仮想パッド1
		SNGamePadPOVUp,				// 方向キー上
		SNGamePadPOVDown,			// 方向キー下
		SNGamePadPOVLeft,			// 方向キー左
		SNGamePadPOVRight,			// 方向キー右
		SNGamePadButton2,			// 決定(Aボタン)
		SNGamePadButton1,			// キャンセル(Bボタン)
		SNGamePadButton4,			// メニュー(Xボタン)
		SNGamePadButton3,			// アクション(Yボタン)
		SNGamePadButton8,			// スタート
		SNGamePadButton7,			// セレクト
		SNGamePadButton5,			// ページ戻し(Lボタン)
		SNGamePadButton6,			// ページ送り(Rボタン)
		SNGamePadAxis3Down,			// Lトリガ(L2ボタン)
		SNGamePadAxis3Up,			// Rトリガ(R2ボタン)
		SNGamePadAxis2Up,			// リストアップ(拡大)
		SNGamePadAxis2Down,			// リストダウン(縮小)


		// 仮想パッド2
		SNGamePadButtonNull,			// 方向キー上
		SNGamePadButtonNull,		    // 方向キー下
		SNGamePadButtonNull,			// 方向キー左
		SNGamePadButtonNull,			// 方向キー右
		SNGamePadButtonNull,			// 決定(Aボタン)
		SNGamePadButtonNull,			// キャンセル(Bボタン)
		SNGamePadButtonNull,			// メニュー(Xボタン)
		SNGamePadButtonNull,			// アクション(Yボタン)
		SNGamePadButtonNull,			// スタート
		SNGamePadButtonNull,			// セレクト
		SNGamePadButtonNull,			// ページ戻し(Lボタン)
		SNGamePadButtonNull,			// ページ送り(Rボタン)
		SNGamePadButtonNull,			// Lトリガ(L2ボタン)
		SNGamePadButtonNull,			// Rトリガ(R2ボタン)
		SNGamePadButtonNull,			// リストアップ(拡大)
		SNGamePadButtonNull,			// リストダウン(縮小)

		//--------------------------------------------
		0,							// チェックサム
	};
};

