#pragma once
#include "SNFrameworkInternal.h"
#include "SNSurface.h"
#include "SNEvent.h"
#include "SNPhase.h"


// シーンクラス
class SNScene : public SNPhase
{
public:
	// コンストラクタ
	SNScene();

	// デストラクタ
	virtual ~SNScene();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行(インターフェース)
	virtual Void Step();

	// 描画処理(インターフェース)
	virtual Void Draw(SNSurface* surface);

	// ローカル座標/サイズ設定
	virtual Void SetRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// ローカル座標/サイズ取得
	virtual SNRect GetRect();

	// 親座標/サイズ設定(グローバル)
	virtual Void SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h);

	// 親座標/サイズ取得(グローバル)
	virtual SNRect GetParentRect();

	// 移動
	// パラメータ：移動量を指定する
	virtual Void Move(Int32 x, Int32 y);

	// リサイズ
	virtual Void Resize(Int32 w, Int32 h);

	// センタリング処理
	virtual Void Centering(Boolean v, Boolean h);

	// グローバル座標/サイズ計算
	virtual SNRect CalcGlobalRect();

	Boolean Enable;			// 有効状態
	Boolean Visible;		// 表示状態
	SNPoint ParentPosition;	// 親座標(グローバル)
	SNSize  ParentSize;		// 親サイズ
	SNPoint LocalPosition;	// 自身の座標(ローカル)
	SNSize  Size;			// 自身のサイズ
	SNTransitionCode TransCode;	// 遷移先コード

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// イベント処理
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean EventProc(SNEventType evt);

	// アプリ通知
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnNotifyEvent();

	// 内部イベント
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnInternalEvent();

	// ゲームパッド
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnGamePad1();

	// ゲームパッド
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnGamePad2();

	// ポインティングデバイス
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnPointingDevice();
	
	// ダイレクトキーボード
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnDirectKeyboard();

	// ダイレクトゲームパッド
	// リターン：true:イベント消費, false:イベント通貨
	virtual Boolean OnDirectGamePad();

	// フレーム処理
	virtual Void OnCycle();

	// 描画処理(実装用)
	virtual Void OnDraw(SNSurface* surface);
};
