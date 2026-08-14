#pragma once
#include "SNFrameworkInternal.h"


// 音楽リソースIDを定義する

enum SNMusicResID
{
	SNMusicResTop,

	SNMusicResMute = SNMusicResTop,
	SNMusicResTitle,

	SNMusicResEnd = SNMusicResTitle,

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

	};
};
