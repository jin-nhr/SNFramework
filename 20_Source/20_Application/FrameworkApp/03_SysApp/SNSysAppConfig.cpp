#include "SNSysAppConfig.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNSystemConfig.h"
#include "SNUserConfig.h"
#include "SNGraphics.h"
#include "SNSystem.h"
#include "SNFocus.h"
#include "SNMath.h"

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

	// 現在設定の退避
	SNUserConfig::BackupUserData();


	return;
}

Void SNSysAppConfig::OnExit()
{
	FocusGp.Exit();

	return;
}

Void SNSysAppConfig::OnCycle()
{
	// ボタンの処理実行
	ProcButton();

	// 状態更新
	UpdateState();

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

Void SNSysAppConfig::UpdateState()
{
	// 画面モード
	GUIButton[GUIButtonWindow].Selected = !SNUserConfig::Data.FullScreen;
	GUIButton[GUIButtonFullScreen].Selected = SNUserConfig::Data.FullScreen;

	// VSYNC
	GUIButton[GUIButtonVSYNCOFF].Selected = !SNUserConfig::Data.VSync;
	GUIButton[GUIButtonVSYNCON].Selected = SNUserConfig::Data.VSync;

	// フィルタ
	GUIButton[GUIButtonFilterOFF].Selected = !SNUserConfig::Data.DrawFilter;
	GUIButton[GUIButtonFilterON].Selected = SNUserConfig::Data.DrawFilter;

	// Master音量
	GUITextEx[GUITextExMasterVolume].SetValue((UInt8)SNMath::Saturate(SNUserConfig::Data.MasterVolume, SNSoundVolMin, SNSoundVolMax));

	// BGM音量
	GUITextEx[GUITextExBGMVolume].SetValue((UInt8)SNMath::Saturate(SNUserConfig::Data.BGMVolume, SNSoundVolMin, SNSoundVolMax));

	// SE音量
	GUITextEx[GUITextExSEVolume].SetValue((UInt8)SNMath::Saturate(SNUserConfig::Data.SEVolume, SNSoundVolMin, SNSoundVolMax));

	return;
}

Void SNSysAppConfig::ProcButton()
{
	// ウインドウ
	if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonWindow]))
	{
		SNUserConfig::Data.FullScreen = false;
	}

	// フルスクリーン
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonFullScreen]))
	{
		SNUserConfig::Data.FullScreen = true;
	}

	// VSYNC OFF
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonVSYNCOFF]))
	{
		SNUserConfig::Data.VSync = false;
	}

	// VSYNC ON
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonVSYNCON]))
	{
		SNUserConfig::Data.VSync = true;
	}

	// 描画フィルタ OFF
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonFilterOFF]))
	{
		SNUserConfig::Data.DrawFilter = false;
	}

	// 描画フィルタ ON
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonFilterON]))
	{
		SNUserConfig::Data.DrawFilter = true;
	}

	// マスター音量
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonMasterVolDown10]))
	{
		SNUserConfig::Data.MasterVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.MasterVolume - 10, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonMasterVolDown1]))
	{
		SNUserConfig::Data.MasterVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.MasterVolume - 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonMasterVolUp1]))
	{
		SNUserConfig::Data.MasterVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.MasterVolume + 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonMasterVolUp10]))
	{
		SNUserConfig::Data.MasterVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.MasterVolume + 10, SNSoundVolMin, SNSoundVolMax);
	}

	// BGM音量
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonBGMVolDown10]))
	{
		SNUserConfig::Data.BGMVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.BGMVolume - 10, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonBGMVolDown1]))
	{
		SNUserConfig::Data.BGMVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.BGMVolume - 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonBGMVolUp1]))
	{
		SNUserConfig::Data.BGMVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.BGMVolume + 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonBGMVolUp10]))
	{
		SNUserConfig::Data.BGMVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.BGMVolume + 10, SNSoundVolMin, SNSoundVolMax);
	}

	// SE音量
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonSEVolDown10]))
	{
		SNUserConfig::Data.SEVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.SEVolume - 10, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonSEVolDown1]))
	{
		SNUserConfig::Data.SEVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.SEVolume - 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonSEVolUp1]))
	{
		SNUserConfig::Data.SEVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.SEVolume + 1, SNSoundVolMin, SNSoundVolMax);
	}
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonSEVolUp10]))
	{
		SNUserConfig::Data.SEVolume = (UInt8)SNMath::Saturate(SNUserConfig::Data.SEVolume + 10, SNSoundVolMin, SNSoundVolMax);
	}

	// P1 ゲームパッド
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonP1GamePad]))
	{
		// パッド入力設定画面へ
	}
	// P1 キーボード
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonP1Keyboard]))
	{
		// キーボード入力設定画面へ
	}
	// P2 ゲームパッド
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonP2GamePad]))
	{
		// パッド入力設定画面へ
	}
	// P2 キーボード
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonP2Keyboard]))
	{
		// キーボード入力設定画面へ
	}

	// デフォルト設定に戻す
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonInitialize]))
	{
		SNUserConfig::InitUserData();
	}

	// OK
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonOK]))
	{
		TransCode = SNTransitionCode0;
	}

	// キャンセル
	else if (FocusGp.JudgeActDecide(&GUIButton[GUIButtonCancel]))
	{
		// 変更前の設定に戻す
		SNUserConfig::RestoreUserData();
		TransCode = SNTransitionCode0;
	}

	else if (FocusGp.JudgeActCancel())
	{
		// 変更前の設定に戻す
		SNUserConfig::RestoreUserData();
		TransCode = SNTransitionCode0;
		SNFocus::CallbackPushButton();
	}

	return;
}
