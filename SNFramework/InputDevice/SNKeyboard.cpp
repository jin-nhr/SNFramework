#include "SNKeyboard.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNWindowsAPI.h"


// キーボードクラス

// コンストラクタ
SNKeyboard::SNKeyboard()
{
	Int loop_cnt;

	// 変数初期化
	for (loop_cnt = 0; loop_cnt < SNKeyCodeNum; loop_cnt++)
	{
		KeyState[loop_cnt] = false;
	}

	return;
}

// デストラクタ
SNKeyboard::~SNKeyboard()
{
	return;
}

// 初期化
Void SNKeyboard::Initialize()
{
	Int loop_cnt;

	// 変数初期化
	for (loop_cnt = 0; loop_cnt < SNKeyCodeNum; loop_cnt++)
	{
		KeyState[loop_cnt] = false;
	}

	return;
}

// 終了処理
Void SNKeyboard::Terminate()
{
	return;
}

// 更新処理
Void SNKeyboard::Update()
{
	Int loop_cnt;
	Boolean* support_flag_array;

	// コンフィグからキー設定を取得
	support_flag_array = SNConfiguration::GetInstance()->ConfigurationData.System.KeySupportFlag;

	for (loop_cnt = 0; loop_cnt < SNKeyCodeNum; loop_cnt++)
	{
		// キー監視対象か確認
		if (support_flag_array[loop_cnt])
		{
			// 対象キーが押されてるか確認
			KeyState[loop_cnt] = (Boolean)((GetAsyncKeyState(loop_cnt) & 0x8000) != 0);
		}
	}

	return;
}

// 状態取得
const Boolean* SNKeyboard::GetState()
{
	// キー状態を還す
	return KeyState;
}
