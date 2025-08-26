#include "SNDebugLayer.h"
#include "../../Configuration/SNConfiguration.h"
#include "../SNApplication.h"

// デバッグレイヤクラス

const String SNDebugLayer::SystemInfoTitle = (String)L"-- System Info --";
const String SNDebugLayer::FPSDispText = (String)L"FPS = %d";
const String SNDebugLayer::ProcTimeDispText = (String)L"ProcTime = %d[ms]";


const String SNDebugLayer::InputDeviceTitle[] =
{
	(String)L"-- InputDeviceID1 --",
	(String)L"-- InputDeviceID2 --"
};

const String SNDebugLayer::InputDeviceText[] =
{
	(String)L"Up = %08x",
	(String)L"Down = %08x",
	(String)L"Left = %08x",
	(String)L"Right = %08x",
	(String)L"Decide = %08x",
	(String)L"Cancel = %08x",
	(String)L"Menu = %08x",
	(String)L"Action = %08x",
	(String)L"Start = %08x",
	(String)L"Select = %08x",
	(String)L"PageUp = %08x",
	(String)L"PageDpwn = %08x",
	(String)L"ListUp = %08x",
	(String)L"ListDown = %08x"
};

const String SNDebugLayer::PointingText[] =
{
	(String)L"-- PointingDevice --",
	(String)L"Point X=%d",
	(String)L"Point Y=%d",
	(String)L"Decide = %d",
	(String)L"Cancel = %d",
	(String)L"WheelUp = %d",
	(String)L"WheelDown = %d",
	(String)L"Move = %d",
	(String)L"Darg = %d",
	(String)L"Drop = %d",
	(String)L"Frick = %d",
	(String)L"VelocityX = %d",
	(String)L"VelocityY = %d"
};


// コンストラクタ
SNDebugLayer::SNDebugLayer()
{
	Int32 cnt;
	Int32 dev;

	// 変数初期化
	for (cnt = 0; cnt < SystemInfoNum; cnt++)
	{
		SystemInfoList[cnt] = nullptr;
	}

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		for (cnt = 0; cnt < InputDeviceDispNum; cnt++)
		{
			InputDeviceList[dev][cnt] = nullptr;
		}
	}

	return;
}

// デストラクタ
SNDebugLayer::~SNDebugLayer()
{
	return;
}

// 初期化
Void SNDebugLayer::Initialize()
{
	Int32 cnt;
	Int32 dev;
	Int32 text_y;

	// システム情報リスト設定
	SystemInfoList[0] = &SystemInfoTitleDisp;
	SystemInfoList[1] = &FPSDisp;
	SystemInfoList[2] = &ProcTimeDisp;

	// コンテナの座標設定
	SystemInfoContainer.SetPosition(20, 20);

	// リストをコンテナに登録/初期化
	SystemInfoContainer.SetGUIPartsInfo(SystemInfoNum, SystemInfoList);
	SystemInfoContainer.Initialize();

	// 子の座標設定
	SystemInfoTitleDisp.SetPosition(0, 0);
	FPSDisp.SetPosition(0, 20);
	ProcTimeDisp.SetPosition(0, 40);

	// 子のテキスト設定
	SystemInfoTitleDisp.SetText(SystemInfoTitle);
	FPSDisp.SetText(FPSDispText);
	ProcTimeDisp.SetText(ProcTimeDispText);

	// 入力デバイスのリスト設定
	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		// タイトル設定
		InputDeviceList[dev][0] = &InputDeviceTitleDisp[dev];

		// ボタン表示部品設定
		for (cnt = 1; cnt < InputDeviceDispNum; cnt++)
		{
			InputDeviceList[dev][cnt] = &InputDeviceDisp[dev][cnt - 1];
		}

		// リストをコンテナに登録/初期化
		InputDeviceContainer[dev].SetPosition(20 + 200 * dev, 100);
		InputDeviceContainer[dev].SetGUIPartsInfo(InputDeviceDispNum, InputDeviceList[dev]);
		InputDeviceContainer[dev].Initialize();
	}

	// 入力デバイスのリスト設定
	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceTitleDisp[dev].SetPosition(0, 0);
		InputDeviceTitleDisp[dev].SetText(InputDeviceTitle[dev]);

		text_y = 20;
		for (cnt = 0; cnt < SNInputButtonNum; cnt++)
		{
			InputDeviceDisp[dev][cnt].SetPosition(0, text_y);
			InputDeviceDisp[dev][cnt].SetText(InputDeviceText[cnt]);
			text_y += 20;
		}
	}


	// マウスのリスト設定
	for (cnt = 0; cnt < PointingDispNum; cnt++)
	{
		PointingList[cnt] = &PointingDisp[cnt];
	}

	PointingContainer.SetPosition(20 + 400, 100);
	PointingContainer.SetGUIPartsInfo(PointingDispNum, PointingList);
	PointingContainer.Initialize();

	for (cnt = 0; cnt < PointingDispNum; cnt++)
	{
		PointingDisp[cnt].SetText(PointingText[cnt]);
	}

	for (cnt = 0; cnt < PointingDispNum; cnt++)
	{
		PointingList[cnt]->SetPosition(0, cnt * 20);
	}

	return;
}

// 終了処理
Void SNDebugLayer::Terminate()
{
	Int32 dev;

	SystemInfoContainer.Terminate();

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceContainer[dev].Terminate();
	}

	PointingContainer.Terminate();

	return;
}

// Entry
Void SNDebugLayer::Entry()
{
	Int32 dev;

	SystemInfoContainer.Entry();

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceContainer[dev].Entry();
	}

	PointingContainer.Entry();

	return;
}

// Exit
Void SNDebugLayer::Exit()
{
	Int32 dev;

	SystemInfoContainer.Exit();

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceContainer[dev].Exit();
	}

	PointingContainer.Exit();

	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNDebugLayer::Step(SNEvent* event)
{
	Int32 cnt;
	Int32 dev;
	UInt32 button_sts;

	FPSDisp.SetValue(SNApplication::GetInstance()->GetFPS());
	ProcTimeDisp.SetValue(SNApplication::GetInstance()->GetProcTime());

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		for (cnt = 0; cnt < SNInputButtonNum; cnt++)
		{
			button_sts = 0;
			button_sts |= (event->InputEvent->ButtonEvent[dev][cnt].Push * 0x10000000);
			button_sts |= (event->InputEvent->ButtonEvent[dev][cnt].Press * 0x01000000);
			button_sts |= (event->InputEvent->ButtonEvent[dev][cnt].LongPress * 0x00100000);
			button_sts |= (event->InputEvent->ButtonEvent[dev][cnt].Repeat * 0x00010000);
			button_sts |= (event->InputEvent->ButtonEvent[dev][cnt].Release * 0x00001000);
			InputDeviceDisp[dev][cnt].SetValue(button_sts);
		}
	}

	PointingDisp[1].SetValue(event->InputEvent->PointingInfo.CurrentPosition.X);
	PointingDisp[2].SetValue(event->InputEvent->PointingInfo.CurrentPosition.Y);
	PointingDisp[3].SetValue(event->InputEvent->PointingEvent.Decide);
	PointingDisp[4].SetValue(event->InputEvent->PointingEvent.Cancel);
	PointingDisp[5].SetValue(event->InputEvent->PointingEvent.WheelUp);
	PointingDisp[6].SetValue(event->InputEvent->PointingEvent.WheelDown);
	PointingDisp[7].SetValue(event->InputEvent->PointingEvent.Move);
	PointingDisp[8].SetValue(event->InputEvent->PointingEvent.Drag);
	PointingDisp[9].SetValue(event->InputEvent->PointingEvent.Drop);
	PointingDisp[10].SetValue(event->InputEvent->PointingEvent.Frick);
	PointingDisp[11].SetValue(event->InputEvent->PointingInfo.FrickVelocity.X);
	PointingDisp[12].SetValue(event->InputEvent->PointingInfo.FrickVelocity.Y);

	SystemInfoContainer.Step(event);

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceContainer[dev].Step(event);
	}

	PointingContainer.Step(event);

	return SNTransitionCodeNo;
}

// 描画処理
Void SNDebugLayer::Draw(SNSurface* surface)
{
	Int32 dev;

	SystemInfoContainer.Draw(surface);

	for (dev = 0; dev < SNInputDeviceNum; dev++)
	{
		InputDeviceContainer[dev].Draw(surface);
	}

	PointingContainer.Draw(surface);
#
	return;
}
