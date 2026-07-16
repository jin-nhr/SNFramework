#include "SNImageCodecAccessor.h"
#include "SNImageCodec.h"
#include "SNSystemTimer.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"


SNImageCodecAccessor::SNImageCodecAccessor()
{
	Operation = SNImageCodecOperationNo;
	Result = SNImageCodecResultIdle;

	return;
}

SNImageCodecAccessor::~SNImageCodecAccessor()
{
	return;
}

SNImageCodecResult SNImageCodecAccessor::GetResult()
{
	return Result;
}

// コールバック
Void SNImageCodecAccessor::Callback()
{
	SNImageCodecResult ret = SNImageCodecResultIdle;

	// 処理中のみ実行
	if (Result == SNImageCodecResultProcessing)
	{
		switch (Operation)
		{
		case SNImageCodecOperationDecode:
			ret = OnOperationImageDecode();
			break;
		case SNImageCodecOperationEncode:
			ret = OnOperationImageEncode();
			break;
		}
	}

	Result = ret;
	
	return;
}

// 命令要求
Void SNImageCodecAccessor::RequestOperation(SNImageCodecOperation ope)
{
	// 命令保存
	Operation = ope;

	// 処理中セット
	Result = SNImageCodecResultProcessing;

	// ストレージスレッドに対して要求をセット
	SNImageCodec::Request(this);
	return;
}

// 命令クリア
Void SNImageCodecAccessor::ClearOperation()
{
	// 命令、結果をクリア
	Operation = SNImageCodecOperationNo;
	Result = SNImageCodecResultIdle;
	return;
}

// 処理完了待ち
Void SNImageCodecAccessor::WaitForOperationComplete()
{
	SNSystemTimer timer(SNSystemConfig::CodecTimeout);

	timer.Start();

	while ((!IsOperationComplete()) && (!timer.CheckTimeout()))
	{
		::Sleep(1);
	}

	return;
}

Boolean SNImageCodecAccessor::IsOperationComplete()
{
	return (GetResult() != SNImageCodecResultProcessing);
}

SNImageCodecResult SNImageCodecAccessor::OnOperationImageDecode()
{
	return SNImageCodecResultIdle;
}

SNImageCodecResult SNImageCodecAccessor::OnOperationImageEncode()
{
	return SNImageCodecResultIdle;
}
