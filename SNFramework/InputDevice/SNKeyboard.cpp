#include "SNKeyboard.h"
#include "../Configuration/SNConfiguration.h"
#include "../System/SNWindowsAPI.h"


// キーボードクラス

// キー状態
Boolean SNKeyboard::KeyState[SNKeyCodeNum] = {0};

// チェック対象キーリスト
UInt8 SNKeyboard::CheckKeyList[SNKeyCodeNum] = {0};

// チェック対象キー数
UInt8 SNKeyboard::CheckKeyNum = 0;



// 初期化
Void SNKeyboard::Initialize()
{
	Int32 loop_cnt;
	Int32 list_cnt = 0;

	// 変数初期化
	for (loop_cnt = 0; loop_cnt < SNKeyCodeNum; loop_cnt++)
	{
		// コンフィグ設定でサポートされるキーの場合
		if (SNConfiguration::SystemConfiguration.KeySupportFlag[loop_cnt])
		{
			// チェック対象リストに登録
			CheckKeyList[list_cnt] = (UInt8)loop_cnt;
			list_cnt++;
		}
	}

	// チェック対象キー数を設定
	CheckKeyNum = (UInt8)list_cnt;

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
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < CheckKeyNum; loop_cnt++)
	{
		// 対象キーが押されてるか確認
		KeyState[CheckKeyList[loop_cnt]] = (Boolean)((GetAsyncKeyState(CheckKeyList[loop_cnt]) & 0x8000) != 0);
	}

	return;
}


