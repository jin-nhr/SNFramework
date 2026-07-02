#include "SNDebugAppRun.h"
#include "SNApplication.h"
#include "SNEvent.h"
#include "SNBitmapFont.h"
#include "SNKeyboard.h"

// デバッグアプリ Run

// コンストラクタ
SNDebugAppRun::SNDebugAppRun()
{
	return;
}

// デストラクタ
SNDebugAppRun::~SNDebugAppRun()
{
	return;
}

Void SNDebugAppRun::OnInitialize()
{
	// システム情報のコンテナ設定
	CntSysInfo.SetSceneNum(3);
	CntSysInfo.SetScene(&LblSysInfoTitle);
	CntSysInfo.SetScene(&LblFPS);
	CntSysInfo.SetScene(&LblProcTime);
	
	CntSysInfo.Initialize();

	// システム情報のテキスト設定
	LblSysInfoTitle.SetText((String)L"-- System Info --");
	LblFPS.SetText((String)L"FPS = %d");
	LblProcTime.SetText((String)L"ProcTime = %d[ms]");

	// システム情報の座標設定
	LblSysInfoTitle.Move(0, 0);
	LblFPS.Move(0, SNBitmapFont::BMCharHeight);
	LblProcTime.Move(0, SNBitmapFont::BMCharHeight * 2);

	return;
}

// 終了処理
Void SNDebugAppRun::OnTerminate()
{
	CntSysInfo.Terminate();

	return;
}

// Entry
Void SNDebugAppRun::OnEntry()
{
	CntSysInfo.Entry();

	return;
}

// Exit
Void SNDebugAppRun::OnExit()
{
	CntSysInfo.Exit();

	return;
}

Void SNDebugAppRun::OnCycle()
{
	static Boolean esc_sts = false;

	LblFPS.SetValue(SNApplication::GetFPS());
	LblProcTime.SetValue(SNApplication::GetProcTime());

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

	CntSysInfo.Step();

	return;
}

// 描画処理
Void SNDebugAppRun::OnDraw(SNSurface* surface)
{
	CntSysInfo.Draw(surface);

	return;
}

