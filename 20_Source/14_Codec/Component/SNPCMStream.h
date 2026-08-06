#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundCodecAccessor.h"
#include "SNMemory.h"
#include "SNStore.h"
#include "SNList.h"
#include "SNCriticalSection.h"

// PCMStreamクラス

class SNPCMStream :public SNSoundCodecAccessor
{
public:
	// コンストラクタ
	SNPCMStream();

	// デストラクタ
	virtual ~SNPCMStream();

	// PCMオープン
	// 対象データを渡す。呼び出し元で処理完了まで保持すること
	virtual Void OpenPCMStream(SNMemory* in_data);

	// PCMクローズ
	virtual Void ClosePCMStream();
	virtual Void ReleaseAllPCMBlock();

	// デコード処理
	virtual Void Decode(Boolean reset);

	// ブロックを取得(使用する側でCSロックしておくこと)
	virtual SNMemory* GetStreamBlock();

	// ブロックを解放(使用する側でCSロックしておくこと)
	virtual Void ReleaseStreamBlock();

	Int32 Channels;
	Int32 SampleRate;
	Int32 BitPerSample;

	SNStore PCMBlockStore;
	SNList  PCMBlockList;
	SNCriticalSection CS;

	SNMemory Source;
	Handle Stream;
	Handle Reader;

	SNMemory* TargetData;
	SNMemory Working;

protected:
	virtual SNSoundCodecResult OnOperationOpenStream();

private:
	static Void* CreateMemoryBlock();
	static Void DeleteMemoryBlock(Void* res);
};
