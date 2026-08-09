#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundCodecAccessor.h"
#include "SNThread.h"
#include "SNList.h"
#include "SNCriticalSection.h"
#include "SNMemory.h"
#include "SNPCM.h"
#include "SNPCMStream.h"


class SNSoundCodec
{
public:
	// 初期化
	static Void Initialize();
	static Void CreateThread();

	// 終了
	static Void Terminate();
	static Void ReleaseThread();

	// スレッドメイン処理
	static Void UserMain();

	// リクエスト
	static Void Request(SNSoundCodecAccessor* request);

	// リクエスト取得
	static SNSoundCodecAccessor* GetRequest();

	// 音声デコード
	static Void Decode(SNMemory* in, SNPCM* out);


	static Boolean ReadSampleOneShot(Handle reader, Handle* sample);

	static Void InitDecodePos(Handle reader);

	static Void LockSampleBuffer(Handle sample, Handle* buffer, UInt8** buffer_adr, UInt32* size);

	static Void CopySample(UInt8* sample, UInt32 src_offset, UInt8* block, UInt32 dst_offset, UInt32 size);

	static Void ReleaseSample(Handle sample, Handle buffer);

	static Void GetPCMMeta(Handle reader, UInt32* ch, UInt32* bits, UInt32* rate);

	// ストリームオープン
	static Void OpenStream(SNMemory* in, SNPCMStream* out);

	// ストリームクローズ(同期呼び出し可)
	static Void CloseStream(SNPCMStream* stream);

private:
	static Handle CreateReaderFromMemory(SNMemory* in);
	static UInt32 DecodeToBlocks(Handle in_reader, SNList* memoryblocklist);
	static Boolean ReadSample(Handle in_reader, Handle* out_sample);
	static Void BuildPCMFromBlocks(SNList& memoryblocklist, UInt32 totalsize, SNPCM* out);
	static Void SetPCMMeta(Handle in_reader, SNPCM* out);

private:
	static SNThread* CodecThread;
	static volatile Boolean ThreadEndRequest;		// スレッド終了要求

	static SNList RequestList;				// リクエストリスト
	static SNCriticalSection ListAccessCS;	// リストアクセス用クリティカルセクション

};
