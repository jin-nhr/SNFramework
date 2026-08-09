#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundCodecAccessor.h"
#include "SNMemory.h"
#include "SNStore.h"
#include "SNList.h"

enum SNPCMStreamDecodePhase
{
	SNPCMStreamDecodePhaseSeek,
	SNPCMStreamDecodePhaseRead,
	SNPCMStreamDecodePhaseSampleLock,
	SNPCMStreamDecodePhaseGetBlock,
	SNPCMStreamDecodePhaseSetBlock,
	SNPCMStreamDecodePhaseSetList
};

struct SNPCMStreamSourceInfo
{
	Handle Sample;
	Handle Buffer;
	UInt8* BufferAdr;
	UInt32 Size;
	UInt32 Offset;
};

struct SNPCMStreamTargetInfo
{
	SNListContainer* Block;
	UInt32 Size;
	UInt32 Used;
};

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

	// デコード開始
	virtual Void StartDecode();

	// デコード処理
	virtual Void Decode();

	// 全デコード
	virtual Void DecodeFull();

	// デコード終了
	virtual Void EndDecode();

	// ブロックを取得
	virtual SNListContainer* GetStreamBlock();

	// ブロックを解放
	virtual Void ReleaseStreamBlock(SNListContainer* block);

	Int32 Channels;
	Int32 SampleRate;
	Int32 BitPerSample;

	SNStore PCMBlockStore;
	SNList  PCMBlockList;

	SNMemory Source;
	Handle Reader;

	SNMemory* TargetData;


	SNPCMStreamDecodePhase DecodePhase;
	SNPCMStreamSourceInfo DecodeSourceInfo;
	SNPCMStreamTargetInfo DecodeTargetInfo;

protected:
	virtual SNSoundCodecResult OnOperationOpenStream();

private:
	static Void* CreateMemoryBlock();
	static Void DeleteMemoryBlock(Void* res);
};
