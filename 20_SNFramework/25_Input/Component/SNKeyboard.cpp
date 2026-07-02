#include "SNKeyboard.h"
#include "SNWindowsAPI.h"
#include "SNInput.h"


// キーボードクラス

// キー状態
Boolean SNKeyboard::KeyState[SNKeyCodeNum] = { 0 };

// サポートキーリスト
SNKeyCode SNKeyboard::SupportKeyList[SNKeyCodeNum] = { SNKeyCodeNull };

// サポートキー数
UInt8 SNKeyboard::SupportKeyNum = 0;



// 初期化
Void SNKeyboard::Initialize()
{
	Int32 loop_cnt;
	Int32 list_cnt = 0;

	// 変数初期化
	for (loop_cnt = 0; loop_cnt < SNKeyCodeNum; loop_cnt++)
	{
		// キー状態をクリア
		KeyState[loop_cnt] = false;

		// コンフィグ設定でサポートされるキーの場合
		if (KeySupportInfo[loop_cnt])
		{
			// サポートキーリストにキーコードを格納し、サポートキー数をカウントアップ
			SupportKeyList[list_cnt] = static_cast<SNKeyCode>(loop_cnt);
			list_cnt++;
		}
	}

	// サポートキー数を設定
	SupportKeyNum = (UInt8)list_cnt;

	return;
}

// 終了処理
Void SNKeyboard::Terminate()
{
	// サポートキー数を0にすることで動作しないようにしておく
	SupportKeyNum = 0;

	return;
}

// 更新処理
Void SNKeyboard::Update()
{
	Int32 loop_cnt;

	// サポートキーを対象に入力状態チェック
	for (loop_cnt = 0; loop_cnt < SupportKeyNum; loop_cnt++)
	{
		// 対象キーが押されてるか確認
		KeyState[SupportKeyList[loop_cnt]] = (Boolean)((GetAsyncKeyState(SupportKeyList[loop_cnt]) & 0x8000) != 0);
	}

	return;
}
