#include "SNDebugApp.h"
#include "SNBitmapFont.h"
#include "SNApplication.h"
#include "SNKeyboard.h"
#include "SNSystemConfig.h"
#include "SNGraphicsDevice.h"
#include "SNGUIDef.h"

// デバッグアプリ

// コンストラクタ
SNDebugApp::SNDebugApp()
{
	return;
}

// デストラクタ
SNDebugApp::~SNDebugApp()
{
	return;
}

Void SNDebugApp::OnInitialize()
{
	Int32 y;

	// システム情報のコンテナ設定
	cntSysInfo.SetSceneNum(7);
	cntSysInfo.SetScene(&txtSysInfoTitle);
	cntSysInfo.SetScene(&txtFPS);
	cntSysInfo.SetScene(&txtProcFPS);
	cntSysInfo.SetScene(&txtSkip);
	cntSysInfo.SetScene(&txtProcTime);
	
	cntSysInfo.Initialize();

	// システム情報のテキスト設定
	txtSysInfoTitle.SetText((String)L"-- System Info --");
	txtFPS.SetText((String)L"FPS = %d");
	txtProcFPS.SetText((String)L"ProcFPS = %d");
	txtSkip.SetText((String)L"Skip = %d");
	txtProcTime.SetText((String)L"ProcTime = %d[ms]");

	txtSysInfoTitle.Color = SNGUIDef::FontColor[SNGUIDef::FontColorSystem];
	txtFPS.Color = SNGUIDef::FontColor[SNGUIDef::FontColorSystem];
	txtProcFPS.Color = SNGUIDef::FontColor[SNGUIDef::FontColorSystem];
	txtSkip.Color = SNGUIDef::FontColor[SNGUIDef::FontColorSystem];
	txtProcTime.Color = SNGUIDef::FontColor[SNGUIDef::FontColorSystem];

	// システム情報の座標設定
	y = 0;

	txtSysInfoTitle.Move(0, y);
	y += SNGUIDef::BMCharHeight;
	txtFPS.Move(0, y);
	y += SNGUIDef::BMCharHeight;
	txtProcFPS.Move(0, y);
	y += SNGUIDef::BMCharHeight;
	txtSkip.Move(0, y);
	y += SNGUIDef::BMCharHeight;
	txtProcTime.Move(0, y);
	y += SNGUIDef::BMCharHeight;

	return;
}

// 終了処理
Void SNDebugApp::OnTerminate()
{
	cntSysInfo.Terminate();

	return;
}

// Entry
Void SNDebugApp::OnEntry()
{
	cntSysInfo.Entry();
	Visible = false;

	return;
}

// Exit
Void SNDebugApp::OnExit()
{
	cntSysInfo.Exit();

	return;
}

Void SNDebugApp::OnCycle()
{
	static Boolean esc_sts = false;

	// デバッグ有効？
	if (SNSystemConfig::DebugEnable)
	{
		txtFPS.SetValue(SNApplication::GetFPS());
		txtProcFPS.SetValue(SNApplication::GetProcFPS());
		txtSkip.SetValue(SNApplication::GetSkipFrame());
		txtProcTime.SetValue(SNApplication::GetProcTime());


		// ESCが押されたらデバッグ表示の反転
		// デバッグアプリはイベントに関係なく直接キー状態を見る
		if (esc_sts != SNKeyboard::KeyState[SNKeyCodeEsc])
		{
			esc_sts = SNKeyboard::KeyState[SNKeyCodeEsc];

			if (esc_sts)
			{
				if (Visible)
				{
					Visible = false;
				}
				else
				{
					Visible = true;
				}
			}
		}
	}
	else
	{
		Visible = false;
	}

	cntSysInfo.Step();

	return;
}


// 描画前処理
Void SNDebugApp::OnPreDraw()
{
	cntSysInfo.PreDraw();

	return;
}



// 描画処理
Void SNDebugApp::OnDraw()
{
	cntSysInfo.Draw();

	return;
}


// 描画後処理
Void SNDebugApp::OnPostDraw()
{
	cntSysInfo.PostDraw();

	return;
}
