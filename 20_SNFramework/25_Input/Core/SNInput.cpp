#include "SNInput.h"
#include "SNConfig.h"
#include "SNApplication.h"
#include "SNKeyboard.h"
#include "SNMouse.h"
#include "SNGamePad.h"
#include "SNVirtualGamePad.h"
#include "SNVirtualPointingDevice.h"
#include "SNVirtualDirectKeyboard.h"
#include "SNVirtualDirectGamePad.h"

// 入力装置クラス

// 初期化処理
Void SNInput::Initialize()
{
	// 実デバイスの初期化
	SNKeyboard::Initialize();
	SNMouse::Initialize();
	SNGamePad::Initialize();

	// 仮想デバイスの初期化
	SNVirtualGamePad::Initialize();
	SNVirtualPointingDevice::Initialize();
	SNVirtualDirectKeyboard::Initialize();
	SNVirtualDirectGamePad::Initialize();

	return;
}

// 起動準備
Void SNInput::Startup()
{
	return;
}

// 実行
Void SNInput::Run()
{
	return;
}

// 終了前処理
Void SNInput::BeforeTerminate()
{
	return;
}

// 終了
Void SNInput::Terminate()
{
	// 仮想デバイスの終了処理
	SNVirtualGamePad::Terminate();
	SNVirtualPointingDevice::Terminate();
	SNVirtualDirectKeyboard::Terminate();
	SNVirtualDirectGamePad::Terminate();

	// 実デバイスの終了処理
	SNKeyboard::Terminate();
	SNMouse::Terminate();
	SNGamePad::Terminate();

	return;
}

// 更新処理
Void SNInput::Update()
{
	// 状態更新
	UpdateState();

	// デバイス更新
	UpdateDevice();

	// 仮想デバイス更新
	UpdateVirtualDevice();

	return;
}

// 状態更新
Void SNInput::UpdateState()
{
	return;
}

// デバイス更新
Void SNInput::UpdateDevice()
{
	// キーボード入力状態更新
	SNKeyboard::Update();

	// マウス入力状態更新
	SNMouse::Update();

	// ゲームパッド入力状態更新
	SNGamePad::Update();

	return;
}


// 仮想デバイス更新
Void SNInput::UpdateVirtualDevice()
{
	// 仮想ゲームパッド更新
	SNVirtualGamePad::Update();

	// 仮想ポインティングデバイス更新
	SNVirtualPointingDevice::Update();

	// 仮想ダイレクトゲームパッド更新
	SNVirtualDirectGamePad::Update();

	// 仮想ダイレクトキーボード起動
	SNVirtualDirectKeyboard::Update();

	return;
}
