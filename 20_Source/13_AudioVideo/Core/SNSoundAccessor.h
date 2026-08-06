#pragma once
#include "SNFrameworkInternal.h"



// サウンドアクセッサ
class SNSoundAccessor
{
public:
	// コンストラクタ
	SNSoundAccessor();

	// デストラクタ
	virtual ~SNSoundAccessor();

	// コールバック
	virtual Void Callback();

	// 命令要求
	virtual Void RequestOperation(SNSoundOperation ope);

	// 命令クリア
	virtual Void ClearOperation();

protected:
	// リクエスト実行関数
	virtual Void OnOperationPlay();
	virtual Void OnOperationStop();

	// リクエストコマンド
	volatile SNSoundOperation Operation;
};
