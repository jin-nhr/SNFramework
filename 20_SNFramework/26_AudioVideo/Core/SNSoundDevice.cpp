#include "SNSoundDevice.h"
#include "SNWindowsAPI.h"


Handle SNSoundDevice::XAudio = nullptr;
Handle SNSoundDevice::MasterVoice = nullptr;


Void SNSoundDevice::Initialize()
{
	InitAudio();

	return;
}


Void SNSoundDevice::InitAudio()
{
	IXAudio2* x_audio = nullptr;
	IXAudio2MasteringVoice* master_voice = nullptr;

	// COM‰Šú‰»
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio¶¬
	XAudio2Create(&x_audio, 0);

	// MasterVoice¶¬
	x_audio->CreateMasteringVoice(&master_voice);

	XAudio = x_audio;
	MasterVoice = master_voice;

	return;
}


Void SNSoundDevice::Terminate()
{
	TermAudio();

	return;
}


Void SNSoundDevice::TermAudio()
{
	IXAudio2* x_audio = (IXAudio2*)XAudio;
	IXAudio2MasteringVoice* master_voice = (IXAudio2MasteringVoice*)MasterVoice;

	// MasterVoice, XAudio”jŠü

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

	// COM ‚ÌI—¹ˆ—
	CoUninitialize();

	return;
}

