#include "SNSoundCodecAccessor.h"
#include "SNSoundCodec.h"
#include "SNSystemTimer.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"


SNSoundCodecAccessor::SNSoundCodecAccessor()
{
	Operation = SNSoundCodecOperationNo;
	Result = SNSoundCodecResultIdle;

	return;
}

SNSoundCodecAccessor::~SNSoundCodecAccessor()
{
	return;
}

SNSoundCodecResult SNSoundCodecAccessor::GetResult()
{
	return Result;
}

// コールバック
Void SNSoundCodecAccessor::Callback()
{
	SNSoundCodecResult ret = SNSoundCodecResultIdle;

	// 処理中のみ実行
	if (Result == SNSoundCodecResultProcessing)
	{
		switch (Operation)
		{
		case SNSoundCodecOperationDecode:
			ret = OnOperationSoundDecode();
			break;

		case SNSoundCodecOperationOpenStream:
			ret = OnOperationOpenStream();
			break;
		}
	}

	Result = ret;
	
	return;
}

// 命令要求
Void SNSoundCodecAccessor::RequestOperation(SNSoundCodecOperation ope)
{
	// 命令保存
	Operation = ope;

	// 処理中セット
	Result = SNSoundCodecResultProcessing;

	// ストレージスレッドに対して要求をセット
	SNSoundCodec::Request(this);
	return;
}

// 命令クリア
Void SNSoundCodecAccessor::ClearOperation()
{
	// 命令、結果をクリア
	Operation = SNSoundCodecOperationNo;
	Result = SNSoundCodecResultIdle;
	return;
}

// 処理完了待ち
Void SNSoundCodecAccessor::WaitForOperationComplete()
{
	SNSystemTimer timer(SNSystemConfig::CodecTimeout);

	timer.Start();

	while ((!IsOperationComplete()) && (!timer.CheckTimeout()))
	{
		::Sleep(1);
	}

	return;
}

Boolean SNSoundCodecAccessor::IsOperationComplete()
{
	return (GetResult() != SNSoundCodecResultProcessing);
}

SNSoundCodecResult SNSoundCodecAccessor::OnOperationSoundDecode()
{
	return SNSoundCodecResultIdle;
}

SNSoundCodecResult SNSoundCodecAccessor::OnOperationOpenStream()
{
	return SNSoundCodecResultIdle;
}
