#pragma once
#include "SNFrameworkInternal.h"

class SNImageCodecAccessor
{
public:
	SNImageCodecAccessor();
	virtual ~SNImageCodecAccessor();

	// 処理結果取得
	virtual SNImageCodecResult GetResult();

	// コールバック
	virtual Void Callback();

	// 命令要求
	virtual Void RequestOperation(SNImageCodecOperation ope);

	// 命令クリア
	virtual Void ClearOperation();

	// 処理完了待ち
	virtual Void WaitForOperationComplete();

	// 処理完了判定
	virtual Boolean IsOperationComplete();

protected:
	virtual SNImageCodecResult OnOperationImageDecode();
	virtual SNImageCodecResult OnOperationImageEncode();

private:
	// 処理結果
	volatile SNImageCodecResult Result;

	// リクエストコマンド
	volatile SNImageCodecOperation Operation;
};
