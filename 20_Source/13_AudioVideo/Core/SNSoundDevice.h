#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"
#include "SNList.h"
#include "SNStore.h"
#include "SNSoundAccessor.h"
#include "SNCriticalSection.h"
#include "SNPCM.h"

class SNSoundDevice
{
public:
	static Void Initialize();
	static Void InitAudio();
	static Void InitStore();

	static Void Update();

	static Void PreTerminate();
	static Void Terminate();
	static Void TermAudio();
	static Void TermStore();


	// スレッドメイン処理
	static Void UserMain();

	// リクエスト
	static Void Request(SNSoundAccessor* request);

	// リクエスト取得
	static SNSoundAccessor* GetRequest();

	// SouceVoice取得
	static SNListContainer* GetSourceVoice();

	// SourceVoice解放
	static Void ReleaseSourceVoice(SNListContainer* source);

	// オペレーション
	static Void Play(SNListContainer* in_source, SNPCM* pcm);
	static Void Stop(SNListContainer* in_source);


	////////////////////////////////////////////////////////
	// Music関連

	// バッファ登録
	static Void SubmitMusicBuffer(SNListContainer* voice, SNMemory* pcm);

	// バッファ数取得
	static Int32 GetBufferNum(SNListContainer* voice);

	// 再生
	static Void MusicPlay(SNListContainer* voice);

	// 一時停止
	static Void MusicPause(SNListContainer* voice);

	// 停止
	static Void MusicStop(SNListContainer* voice);


	static Handle XAudio;
	static Handle MasterVoice;

private:
	static SNThread* SoundThread;				// スレッド
	static volatile Boolean ThreadEndRequest;	// スレッド終了要求

	static SNList RequestList;					// リクエストリスト
	static SNCriticalSection ListAccessCS;		// リストアクセス用クリティカルセクション

	static SNStore SourceVoiceStore;


	static Void* CreateSourceVoice();
	static Void DeleteSourceVoice(Void* res);
};

