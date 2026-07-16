#include "SNApplicationLoading.h"
#include "SNWindowsAPI.h"
#include "SNConfig.h"


// アプリケーション(ローディング)


// コンストラクタ
SNApplicationLoading::SNApplicationLoading()
{
	// シーン登録
	SetSceneNum(3);
	SetScene(&Fader);
	SetScene(&Message);
	SetScene(&BackGround);

	Phase = 0;

	return;
}

// デストラクタ
SNApplicationLoading::~SNApplicationLoading()
{
	return;
}

// 初期化
Void SNApplicationLoading::OnInitialize()
{
	SNRect rect = CalcGlobalRect();

	// フェーダー
	Fader.SetRect(0, 0, Size.Width, Size.Height);
	Fader.SetColor(SNColorBlack);
	Fader.SetFadeType(SNGUIFadeTypeCloss);
	Fader.SetAlpha(SNAlphaMax);

	// ラベル
	Message.SetRect(0, 0, Size.Width, Size.Height);
	Message.SetText((String)L"SN Framework");
	Message.Centering(true, true);	

	// 背景
	BackGround.SetRect(0, 0, Size.Width, Size.Height);
	BackGround.SetColor(SNColorBlack);

	return;
}

// 終了処理
Void SNApplicationLoading::OnTerminate()
{
	return;
}

// Entry
Void SNApplicationLoading::OnEntry()
{
	return;
}

// Exit
Void SNApplicationLoading::OnExit()
{
	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNApplicationLoading::OnStep(SNEvent* event)
{
	SNTransitionCode ret = SNTransitionCodeStay;

	switch (Phase)
	{
	case 0:
		PhaseTimer.Start(InitialTime);
		Phase++;
		break;

	case 1:
		// 初期黒画終了
		if (PhaseTimer.IsTimeout())
		{
			// フェードイン開始
			Fader.FadeIn(FadeInTime);
			Phase++;
		}
		break;

	case 2:
		// フェードイン完了
		if (Fader.IsComplete())
		{
			PhaseTimer.Start(LogoTime);
			Phase++;
		}
		break;

	case 3:
		// タイムアウト
		if (PhaseTimer.IsTimeout())
		{
			Fader.FadeOut(FadeOutTime);
			Phase++;
		}
		break;

	case 4:
		// フェードアウト完了
		if (Fader.IsComplete())
		{
			Fader.SetAlpha(255);
			PhaseTimer.Start(AfterTime);
			Phase++;
		}
		break;

	case 5:
		// タイムアウト
		if (PhaseTimer.IsTimeout())
		{
			ret = SNTransitionCode1;
		}
		break;
	}

	return ret;
}

// 描画処理
Void SNApplicationLoading::OnDraw(SNSurface* grc)
{
	return;
}
