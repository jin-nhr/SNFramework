#pragma once
#include "SNFrameworkInternal.h"

class SNSoundCodecAccessor
{
public:
	SNSoundCodecAccessor();
	virtual ~SNSoundCodecAccessor();

	// 処理結果取得
	virtual SNSoundCodecResult GetResult();

	// コールバック
	virtual Void Callback();

	// 命令要求
	virtual Void RequestOperation(SNSoundCodecOperation ope);

	// 命令クリア
	virtual Void ClearOperation();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();

	// 処理完了判定
	virtual Boolean IsOperationComplete();

protected:
	virtual SNSoundCodecResult OnOperationSoundDecode();
	virtual SNSoundCodecResult OnOperationOpenStream();

private:
	// 処理結果
	volatile SNSoundCodecResult Result;

	// リクエストコマンド
	volatile SNSoundCodecOperation Operation;
};
