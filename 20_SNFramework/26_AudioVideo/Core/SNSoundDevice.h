#pragma once
#include "SNFrameworkInternal.h"



class SNSoundDevice
{
public:
	static Void Initialize();
	static Void InitAudio();



	static Void Terminate();
	static Void TermAudio();


	static Handle XAudio;
	static Handle MasterVoice;
};

