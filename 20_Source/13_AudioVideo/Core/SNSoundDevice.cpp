#include "SNSoundDevice.h"
#include "SNWindowsAPI.h"
#include "SNSoundThread.h"
#include "SNSystemConfig.h"
#include "SNAutoResource.h"
#include "SNUserConfig.h"

Handle SNSoundDevice::XAudio = nullptr;
Handle SNSoundDevice::MasterVoice = nullptr;

SNThread* SNSoundDevice::SoundThread;				// スレッド
volatile Boolean SNSoundDevice::ThreadEndRequest = false;	// スレッド終了要求

SNList SNSoundDevice::RequestList;					// リクエストリスト
SNCriticalSection SNSoundDevice::ListAccessCS;		// リストアクセス用クリティカルセクション

SNStore SNSoundDevice::SourceVoiceStore;


Void SNSoundDevice::Initialize()
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


Void SNSoundDevice::InitAudio()
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

Void SNSoundDevice::InitStore()
{
	SourceVoiceStore.CreateResourceFunc = SNSoundDevice::CreateSourceVoice;
	SourceVoiceStore.DeleteResourceFunc = SNSoundDevice::DeleteSourceVoice;
	SourceVoiceStore.CreateStore(SNSystemConfig::SoundEffectMaxCh);

	return;
}

Void SNSoundDevice::Update()
{
	IXAudio2MasteringVoice* voice = (IXAudio2MasteringVoice*)MasterVoice;
	voice->SetVolume(SNUserConfig::Data.MasterVolume / 100.0f);

	return;
}


Void SNSoundDevice::PreTerminate()
{
	// Voiceを止めるためにStore破棄
	TermStore();

	return;
}

Void SNSoundDevice::Terminate()
{
	// 終了要求セット
	ThreadEndRequest = true;

	// 終了待ち(MAX1秒)
	SoundThread->WaitForThreadEnd();

	TermAudio();

	return;
}

Void SNSoundDevice::TermAudio()
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

Void SNSoundDevice::TermStore()
{
	SourceVoiceStore.DeleteStore();

	return;
}

// スレッドメイン
Void SNSoundDevice::UserMain()
{
	SNSoundAccessor* request;

	// ストレージスレッドメインループ
	while (true)
	{
		// 終了要求がセットされたら抜ける
		if (SNSoundDevice::ThreadEndRequest)
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
Void SNSoundDevice::Request(SNSoundAccessor* request)
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
SNSoundAccessor* SNSoundDevice::GetRequest()
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

SNListContainer* SNSoundDevice::GetSourceVoice()
{
	return SourceVoiceStore.GetResource();
}

Void SNSoundDevice::ReleaseSourceVoice(SNListContainer* source)
{
	SourceVoiceStore.ReleaseResource(source);

	return;
}

Void SNSoundDevice::Play(SNListContainer* in_source, SNPCM* pcm)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)in_source->UserData;
	XAUDIO2_BUFFER buffer = {};

	// 前の残りがあるかもなのでStop, FlushBufferしておく
	source_voice->Stop();
	source_voice->FlushSourceBuffers();

	buffer.AudioBytes = pcm->GetSize();
	buffer.pAudioData = pcm->GetPCM();
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	source_voice->SetVolume(SNUserConfig::Data.SEVolume / 100.0f);
	source_voice->SubmitSourceBuffer(&buffer);
	source_voice->Start();

	return;
}

Void SNSoundDevice::Stop(SNListContainer* in_source)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)in_source->UserData;

	// 次の再生時にゴミが残らないようにバッファをクリア
	source_voice->Stop();
	source_voice->FlushSourceBuffers();

	return;
}


Void SNSoundDevice::SubmitMusicBuffer(SNListContainer* voice, SNMemory* pcm)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)voice->UserData;
	XAUDIO2_BUFFER buffer = {};

	buffer.AudioBytes = pcm->GetSize();
	buffer.pAudioData = (BYTE*)pcm->GetAddress();
	buffer.Flags = 0;

	source_voice->SetVolume(SNUserConfig::Data.BGMVolume / 100.0f);
	source_voice->SubmitSourceBuffer(&buffer);

	return;
}

// バッファ数取得
Int32 SNSoundDevice::GetBufferNum(SNListContainer* voice)
{
	Int32 ret = 0;
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)voice->UserData;
	XAUDIO2_VOICE_STATE state;

	source_voice->GetState(&state);

	ret = state.BuffersQueued;

	return ret;
}

// 再生
Void SNSoundDevice::MusicPlay(SNListContainer* voice)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)voice->UserData;

	source_voice->SetVolume(SNUserConfig::Data.BGMVolume / 100.0f);
	source_voice->Start();

	return;
}

// 一時停止
Void SNSoundDevice::MusicPause(SNListContainer* voice)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)voice->UserData;

	source_voice->Stop();

	return;
}

// 停止
Void SNSoundDevice:: MusicStop(SNListContainer* voice)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)voice->UserData;

	// 次の再生時にゴミが残らないようにバッファをクリア
	source_voice->Stop();
	source_voice->FlushSourceBuffers();

	return;
}


Void* SNSoundDevice::CreateSourceVoice()
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

Void SNSoundDevice::DeleteSourceVoice(Void* res)
{
	IXAudio2SourceVoice* source_voice = (IXAudio2SourceVoice*)res;

	source_voice->Stop();
	source_voice->FlushSourceBuffers();
	source_voice->DestroyVoice();

	return;
}
