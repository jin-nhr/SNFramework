#pragma once
#include "SNFrameworkInternal.h"


// 音楽リソースIDを定義する

enum SNMusicResID
{
	SNMusicResTop,

	SNMusicResMute = SNMusicResTop,
	SNMusicResTitle,
	SNMusicResEnterWorld,

	SNMusicResEnd = SNMusicResEnterWorld,

	SNMusicResNum,
};

class SNMusicResource
{
public:
	static constexpr String ResourceInfoTable[SNMusicResNum] =
	{
		//(String)L"Mute.mp3",
		(String)L"Mute.mp3",
		(String)L"yuruyakanaasayake.mp3",
		(String)L"latenightsnow.mp3",
	};
};
