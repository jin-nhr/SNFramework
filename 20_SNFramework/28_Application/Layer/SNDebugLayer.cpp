#include "SNDebugLayer.h"
#include "SNConfig.h"
#include "SNApplication.h"
#include "SNBitmapFont.h"

// デバッグレイヤクラス

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
	(String)L"PagePrev = %08x",
	(String)L"PageNext = %08x",
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
	return;
}

// デストラクタ
SNDebugLayer::~SNDebugLayer()
{
	return;
}

// 初期化
Void SNDebugLayer::OnInitialize()
{
	// システム情報のコンテナ設定
	ContainerSystemInfo.SetSceneNum(3);
	ContainerSystemInfo.SetScene(&LabelTitleSysInfo);
	ContainerSystemInfo.SetScene(&LabelFPS);
	ContainerSystemInfo.SetScene(&LabelProcTime);

	ContainerSystemInfo.Initialize();

	// システム情報のテキスト設定
	LabelTitleSysInfo.SetText((String)L"-- System Info --");
	LabelFPS.SetText((String)L"FPS = %d");
	LabelProcTime.SetText((String)L"ProcTime = %d[ms]");

	// システム情報の座標設定
	LabelTitleSysInfo.Move(0, 0);
	LabelFPS.Move(0, SNBitmapFont::BMCharHeight);
	LabelProcTime.Move(0, SNBitmapFont::BMCharHeight * 2);

	return;
}

// 終了処理
Void SNDebugLayer::OnTerminate()
{

	return;
}

// Entry
Void SNDebugLayer::OnEntry()
{
	ContainerSystemInfo.Entry();

	return;
}

// Exit
Void SNDebugLayer::OnExit()
{
	ContainerSystemInfo.Exit();

	return;
}

// 1フレーム実行
// リターン：遷移先コード
//           -1:遷移なし
//           0~:状態クラス毎に規程する遷移先コード
SNTransitionCode SNDebugLayer::OnStep(SNEvent* event)
{
	// FPS, 処理時間更新
	LabelFPS.SetValue(SNApplication::GetFPS());
	LabelProcTime.SetValue(SNApplication::GetProcTime());

	ContainerSystemInfo.Step(event);

	return SNTransitionCodeStay;
}

// 描画処理
Void SNDebugLayer::OnDraw(SNSurface* grc)
{
	ContainerSystemInfo.Draw(grc);

	return;
}
