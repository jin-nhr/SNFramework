#include "SNSysAppConfig.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFocus.h"

SNRect SNSysAppConfig::GUITextRect[GUITextNum] =
{
	{20, 20, 240, 24},
	{36, 60, 240, 24},
	{36, 100, 240, 24},
	{36, 140, 240, 24},
	{36, 180, 240, 24},
	{36, 220, 240, 24},
	{36, 260, 240, 24},
	{36, 300, 240, 24},
	{36, 340, 240, 24},
};

SNRect SNSysAppConfig::GUITextExRect[GUITextExNum] =
{
	{384, 180, 64, 24},
	{384, 220, 64, 24},
	{384, 260, 64, 24},
};

SNRect SNSysAppConfig::GUIButtonRect[GUIButtonNum] =
{
	{260, 56, 144, 32},
	{412, 56, 144, 32},
	{260, 96, 144, 32},
	{412, 96, 144, 32},
	{260, 136, 144, 32},
	{412, 136, 144, 32},
	{260, 176, 56, 32},
	{320, 176, 56, 32},
	{440, 176, 56, 32},
	{500, 176, 56, 32},
	{260, 216, 56, 32},
	{320, 216, 56, 32},
	{440, 216, 56, 32},
	{500, 216, 56, 32},
	{260, 256, 56, 32},
	{320, 256, 56, 32},
	{440, 256, 56, 32},
	{500, 256, 56, 32},
	{260, 296, 144, 32},
	{412, 296, 144, 32},
	{260, 336, 144, 32},
	{412, 336, 144, 32},
	{260, 376, 296, 32},
	{260, 416, 144, 32},
	{412, 416, 144, 32},
};

String SNSysAppConfig::GUITextString[GUITextNum] =
{
	(String)L"[システムせってい]",
	(String)L"がめんモード",
	(String)L"VSYNC",
	(String)L"びょうがフィルタ",
	(String)L"マスターおんりょう",
	(String)L"BGMおんりょう",
	(String)L"SEおんりょう",
	(String)L"プレイヤー1にゅうりょく",
	(String)L"プレイヤー2にゅうりょく",
};

String SNSysAppConfig::GUITextExString[GUITextExNum] =
{
	(String)L"%3d",
	(String)L"%3d",
	(String)L"%3d",
};

String SNSysAppConfig::GUIButtonString[GUIButtonNum] =
{
	(String)L"ウインドウ",
	(String)L"フルスクリーン",
	(String)L"OFF",
	(String)L"ON",
	(String)L"OFF",
	(String)L"ON",
	(String)L"-10",
	(String)L"-1",
	(String)L"+1",
	(String)L"+10",
	(String)L"-10",
	(String)L"-1",
	(String)L"+1",
	(String)L"+10",
	(String)L"-10",
	(String)L"-1",
	(String)L"+1",
	(String)L"+10",
	(String)L"ゲームパッドわりあて",
	(String)L"キーボードわりあて",
	(String)L"ゲームパッドわりあて",
	(String)L"キーボードわりあて",
	(String)L"デフォルトせっていにもどす",
	(String)L"OK",
	(String)L"キャンセル",
};

// コンストラクタ
SNSysAppConfig::SNSysAppConfig()
{
	return;
}

// デストラクタ
SNSysAppConfig::~SNSysAppConfig()
{
	return;
}

Void SNSysAppConfig::OnInitialize()
{
	Int32 cnt;

	SetRect(16, 16, 588, 480);
	Centering(true, true);
	SetSceneNum(GUITextNum + GUITextExNum + GUIButtonNum);

	for (cnt = 0; cnt < GUITextNum; cnt++)
	{
		GUIText[cnt].Initialize();
		GUIText[cnt].SetRect(
			GUITextRect[cnt].PointX,
			GUITextRect[cnt].PointY,
			GUITextRect[cnt].Width,
			GUITextRect[cnt].Height);
		GUIText[cnt].SetText(GUITextString[cnt]);
		SetScene(&GUIText[cnt]);
	}

	for (cnt = 0; cnt < GUITextExNum; cnt++)
	{
		GUITextEx[cnt].Initialize();
		GUITextEx[cnt].SetRect(
			GUITextExRect[cnt].PointX,
			GUITextExRect[cnt].PointY,
			GUITextExRect[cnt].Width,
			GUITextExRect[cnt].Height);
		GUITextEx[cnt].SetText(GUITextExString[cnt]);
		GUITextEx[cnt].SetValue(100);
		SetScene(&GUITextEx[cnt]);
	}

	FocusGp.StartRegister(SNFocusLayerSysApp, GUIButtonNum, false, false);

	for (cnt = 0; cnt < GUIButtonNum; cnt++)
	{
		GUIButton[cnt].Initialize();
		GUIButton[cnt].SetRect(
			GUIButtonRect[cnt].PointX,
			GUIButtonRect[cnt].PointY,
			GUIButtonRect[cnt].Width,
			GUIButtonRect[cnt].Height);
		GUIButton[cnt].Caption.SetText(GUIButtonString[cnt]);
		GUIButton[cnt].Caption.Centering(true, true);
		SetScene(&GUIButton[cnt]);
		FocusGp.RegisterButton(&GUIButton[cnt]);
	}

	FocusGp.EndRegister();

	return;
}

Void SNSysAppConfig::OnTerminate()
{
	return;
}


// Entry
Void SNSysAppConfig::OnEntry()
{
	FocusGp.Entry();

	if (SNMute::GetMuteSts() == SNMuteStsOn)
	{
		SNMute::SetMute(false, true);
	}

	return;
}

Void SNSysAppConfig::OnExit()
{

	FocusGp.Exit();

	return;
}

Void SNSysAppConfig::OnCycle()
{

	return;
}


// イベント通知
Boolean SNSysAppConfig::OnNotifyEvent()
{
	// 終了通知
	if (SNApplication::GetEventInfo(SNEventExitApplication))
	{
		TransCode = SNTransitionCode3;

		// 処理対象のイベント種別をキャンセル
		SNEvent::EventExist[SNEventTypeNotifyEvent] = false;

		SNFocus::CallbackPushButton();
	}

	return false;
}

