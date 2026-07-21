#include "SNEvent.h"
#include "SNApplication.h"
#include "SNInput.h"
#include "SNVirtualGamePad.h"
#include "SNVirtualPointingDevice.h"
#include "SNVirtualDirectGamePad.h"
#include "SNVirtualDirectKeyboard.h"

Boolean SNEvent::EventExist[SNEventTypeNum];		// イベント有無
Boolean SNEvent::EventResult[SNEventResultNum];		// イベント処理結果
Boolean SNEvent::InternalEvent[SNEventResultNum];	// 内部イベント


// 初期化
Void SNEvent::Initialize()
{
	Int32 cnt;

	// 初期化
	for (cnt = 0; cnt < SNEventTypeNum; cnt++)
	{
		EventExist[cnt] = false;
	}

	for (cnt = 0; cnt < SNEventResultNum; cnt++)
	{
		EventResult[cnt] = false;
	}

	for (cnt = 0; cnt < SNEventResultNum; cnt++)
	{
		InternalEvent[cnt] = false;
	}

	return;
}

// 終了処理
Void SNEvent::Terminate()
{
	return;
}

// 更新処理
Void SNEvent::Update()
{
	Int32 cnt;

	// イベント有無クリア
	for (cnt = 0; cnt < SNEventTypeNum; cnt++)
	{
		EventExist[cnt] = false;
	}

	// アプリ通知
	for (cnt = 0; cnt < SNEventNum; cnt++)
	{
		if (SNApplication::GetEventInfo((SNApplicationEvent)cnt))
		{
			EventExist[SNEventTypeNotifyEvent] = true;
			break;
		}
	}

	// 内部イベント
	for (cnt = 0; cnt < SNEventResultNum; cnt++)
	{
		// リザルトを内部イベントにコピー
		InternalEvent[cnt] = EventResult[cnt];

		if (InternalEvent[cnt])
		{
			EventExist[SNEventTypeInternalEvent] = true;
		}

		// リザルトクリア
		EventResult[cnt] = false;
	}

	// ゲームパッド1
	if (SNVirtualGamePad::EventExist[SNVirtualGamePadID1])
	{
		EventExist[SNEventTypeVirtualGamePad1] = true;
	}

	// ゲームパッド2
	if (SNVirtualGamePad::EventExist[SNVirtualGamePadID2])
	{
		EventExist[SNEventTypeVirtualGamePad2] = true;
	}

	// ポインティングデバイス
	if (SNVirtualPointingDevice::EventExist)
	{
		EventExist[SNEventTypeVirtualPointing] = true;
	}

	// ダイレクトキーボード
	if (SNVirtualDirectKeyboard::Active)
	{
		EventExist[SNEventTypeVirtualDirectKeyboard] = true;
	}

	// ダイレクトゲームパッド
	if (SNVirtualDirectGamePad::Active)
	{
		EventExist[SNEventTypeVirtualDirectGamePad] = true;
	}

	return;
}
