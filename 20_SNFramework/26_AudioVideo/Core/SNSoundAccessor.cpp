#include "SNSoundAccessor.h"
#include "SNSound.h"
#include "SNWindowsAPI.h"


// サウンドアクセサ

// コンストラクタ
SNSoundAccessor::SNSoundAccessor()
{
	// 変数初期化
	Operation = SNSoundOperationNo;

	return;
}

// デストラクタ
SNSoundAccessor::~SNSoundAccessor()
{
	return;
}

// コールバック
Void SNSoundAccessor::Callback()
{
	switch (Operation)
	{
	case SNSoundOperationPlay:
		OnOperationPlay();
		break;

	case SNSoundOperationStop:
		OnOperationStop();
		break;
	}

	return;
}

// 命令要求
Void SNSoundAccessor::RequestOperation(SNSoundOperation ope)
{
	// 命令保存
	Operation = ope;

	// スレッドに対して要求をセット
	SNSound::Request(this);

	return;
}

// 命令クリア
Void SNSoundAccessor::ClearOperation()
{
	// 命令をクリア
	Operation = SNSoundOperationNo;

	return;
}

// リクエスト実行関数
Void SNSoundAccessor::OnOperationPlay()
{
	return;
}

Void SNSoundAccessor::OnOperationStop()
{
	return;
}
