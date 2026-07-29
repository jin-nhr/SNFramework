#include "SNSound.h"
#include "SNWindowsAPI.h"
#include "SNSoundThread.h"
#include "SNSystemConfig.h"
#include "SNAutoResource.h"

Handle SNSound::XAudio = nullptr;
Handle SNSound::MasterVoice = nullptr;

SNThread* SNSound::SoundThread;				// スレッド
volatile Boolean SNSound::ThreadEndRequest = false;	// スレッド終了要求

SNList SNSound::RequestList;					// リクエストリスト
SNCriticalSection SNSound::ListAccessCS;		// リストアクセス用クリティカルセクション

SNStore SNSound::SourceVoiceStore;


Void SNSound::Initialize()
{
	InitAudio();

	InitStore();

	// リスト確保
	RequestList.Allocate(SNSystemConfig::SoundRequestMax);

	// リストアクセス用CS
	ListAccessCS.Initialize();

	// スレッドクラス生成
	SoundThread = new SNSoundThread;

	// スレッド起動
	SoundThread->Run();

	return;
}


Void SNSound::InitAudio()
{
	IXAudio2* x_audio = nullptr;
	IXAudio2MasteringVoice* master_voice = nullptr;

	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&x_audio, 0);

	// MasterVoice生成
	x_audio->CreateMasteringVoice(&master_voice);

	XAudio = x_audio;
	MasterVoice = master_voice;

	return;
}

Void SNSound::InitStore()
{
	SourceVoiceStore.CreateResourceFunc = SNSound::CreateSourceVoice;
	SourceVoiceStore.DeleteResourceFunc = SNSound::DeleteSourceVoice;
	SourceVoiceStore.CreateStore(SNSystemConfig::SoundEffectMaxCh);

	return;
}


Void SNSound::Terminate()
{
	// 終了要求セット
	ThreadEndRequest = true;

	// 終了待ち(MAX1秒)
	SoundThread->WaitForThreadEnd();

	TermStore();

	TermAudio();

	return;
}

Void SNSound::TermAudio()
{
	IXAudio2* x_audio = (IXAudio2*)XAudio;
	IXAudio2MasteringVoice* master_voice = (IXAudio2MasteringVoice*)MasterVoice;

	// MasterVoice, XAudio破棄

	if (master_voice != nullptr)
	{
		master_voice->DestroyVoice();
		MasterVoice = nullptr;
	}

	if (x_audio != nullptr)
	{
		x_audio->Release();
		XAudio = nullptr;
	}

	// COM の終了処理
	CoUninitialize();

	return;
}

Void SNSound::TermStore()
{
	SourceVoiceStore.DeleteStore();

	return;
}

// スレッドメイン
Void SNSound::UserMain()
{
	SNSoundAccessor* request;

	// ストレージスレッドメインループ
	while (true)
	{
		// 終了要求がセットされたら抜ける
		if (SNSound::ThreadEndRequest)
		{
			break;
		}

		// リクエスト取得
		request = GetRequest();
		if (request != nullptr)
		{
			request->Callback();
		}

		// 処理後、いったんCPU解放
		::Sleep(1);
	}

	return;
}


// リクエスト
Void SNSound::Request(SNSoundAccessor* request)
{
	SNListContainer* list;

	{
		SNAutoResource cs(&ListAccessCS);

		// リストの最後にリクエストを追加
		list = RequestList.InsertLast();
		if (list != nullptr)
		{
			list->UserData = (Void*)request;
		}
	}

	return;
}

// リクエスト取得
SNSoundAccessor* SNSound::GetRequest()
{
	SNListContainer* list;
	SNSoundAccessor* ret = nullptr;

	{
		SNAutoResource cs(&ListAccessCS);

		// リスト先頭からリクエストを取得しリスト削除
		list = RequestList.GetTop();

		if (list != nullptr)
		{
			ret = (SNSoundAccessor*)list->UserData;
			RequestList.RemoveTop();
		}
	}

	return ret;
}

SNListContainer* SNSound::GetSourceVoice()
{
	return SourceVoiceStore.GetResource();
}

Void SNSound::ReleaseSourceVoice(SNListContainer* source)
{
	SourceVoiceStore.ReleaseResource(source);

	return;
}

Void SNSound::Play(SNListContainer* in_source, SNPCM* pcm)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)in_source->UserData;
	XAUDIO2_BUFFER buffer = {};

	// 前の残りがあるかもなのでStop, FlushBufferしておく
	source_voice->Stop();
	source_voice->FlushSourceBuffers();

	buffer.AudioBytes = pcm->GetSize();
	buffer.pAudioData = pcm->GetPCM();
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	source_voice->SubmitSourceBuffer(&buffer);

	source_voice->Start();

	return;
}

Void SNSound::Stop(SNListContainer* in_source)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)in_source->UserData;

	source_voice->Stop();

	return;
}

Void* SNSound::CreateSourceVoice()
{
	IXAudio2* xaudio2 = (IXAudio2*)XAudio;
	IXAudio2SourceVoice* source_voice = nullptr;
	WAVEFORMATEX wfx = {};

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = SNSystemConfig::PCMChannel;
	wfx.nSamplesPerSec = SNSystemConfig::PCMSampleRate;
	wfx.wBitsPerSample = SNSystemConfig::PCMBitPerSample; 
	wfx.nBlockAlign = wfx.nChannels * (wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	xaudio2->CreateSourceVoice(&source_voice, &wfx);

	return source_voice;
}

Void SNSound::DeleteSourceVoice(Void* res)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)res;

	source_voice->Stop();
	source_voice->FlushSourceBuffers();
	source_voice->DestroyVoice();

	return;
}
