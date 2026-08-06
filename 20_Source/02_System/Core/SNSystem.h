#pragma once
#include "SNFrameworkInternal.h"
#include "SNWindow.h"
#include "SNMutex.h"

// システムクラス
// SNFramework全体の起動、実行、終了制御を行う
class SNSystem
{
public:
	// 初期化処理
	// パラメータ：WinMainのパラメータを渡す
	// SNFramework全体の初期化を行う
	// 各ブロック内に閉じた処理とし、ブロック間の参照は禁止
	static Void Initialize(
		Handle application_handle,
		Handle application_prev_handle,
		String command_line,
		Int32 show_command);
	
	// 起動準備
	// SNFramework全体の起動準備を行う
	// 主に他ブロックとの連携を必要とする起動処理を行う
	static Void Startup();
	
	// 実行
	// 各ブロックを起動後、アプリケーション終了までメインループを実行する
	// 定常処理が必要なブロックは別途スレッドを起動し制御が止まらないようにする
	static Void Run();

	// 終了前処理
	// 各ブロックの制御を停止する
	// ブロック間で相互参照するリソースの解放は禁止
	static Void BeforeTerminate();
	
	// 終了
	// 全リソース解放を含む SNFramework の終了処理を行う
	static Void Terminate();

	// 終了通知
	// 何らかのアプリ終了要因を検知したときにウインドウに終了を通知する
	static Void NoticeExitApplication();

	// アプリケーションハンドル取得
	static Handle GetApplicationHandle();

	// 画面座標→クライアント座標変換
	// パラメータに変換結果を返す
	static Void ScreenToClient(SNPoint* point);

	// 多重起動チェック
	// システム状態を加味して多重起動NGかどうかを判定する
	// true:多重起動NG, false:多重起動許可or単一起動
	static Boolean CheckDuplicateInstance();

	static Handle	ApplicationHandle;		// アプリケーションハンドル
	static Handle	ApplicationPrevHandle;	// 既存アプリケーションハンドル
	static String	CommandLine;			// コマンドライン
	static Int32	ShowCommand;			// 表示コマンド
	static SNMutex	DualBootChecker;		// 二重起動チェッカー
};

