#pragma once
#include "SNFrameworkInternal.h"



enum SNEventType
{
	SNEventTypeNotifyEvent,				// 通知イベント               (SNApplication)
	SNEventTypeInternalEvent,			// 内部イベント               (SNEvent)
	SNEventTypeVirtualGamePad1,			// ゲームパッド1入力          (SNVirtualGamePad1)
	SNEventTypeVirtualGamePad2,			// ゲームパッド2入力          (SNVirtualGamePad2)
	SNEventTypeVirtualPointing,			// ポインティングデバイス入力 (SNVirtualPointingDevice)
	SNEventTypeVirtualDirectKeyboard,	// ダイレクトキーボード入力   (SNVirtualDirectKeyboard)
	SNEventTypeVirtualDirectGamePad,	// ダイレクトゲームパッド入力 (SNVritualDirectGamePad)
	SNEventTypeNum
};

enum SNEventResult
{
	SNEventResultSystemTop = 0,
	SNEventResultExitApplication,		// アプリケーション終了通知
	SNEventResultSysAppStartup,			// システムアプリ起動開始
	SNEventResultPendResize,				// リサイズ保留
	SNEventResultUserTop = 32,			// ユーザーイベント先頭
	SNEventResultUserLast = 63,			// ユーザーイベント最終
	SNEventResultNum
};


// イベントクラス
// 各ブロックのイベントを統合管理する
// 内部イベントについては前回のApplication実行結果をループバックする
class SNEvent
{
public:
	// 初期化
	static Void Initialize();

	// 終了処理
	static Void Terminate();

	// 更新処理
	static Void Update();


	static Boolean EventExist[SNEventTypeNum];		// イベント有無
	static Boolean EventResult[SNEventResultNum];	// イベント処理結果
	static Boolean InternalEvent[SNEventResultNum];	// 内部イベント
private:


};
