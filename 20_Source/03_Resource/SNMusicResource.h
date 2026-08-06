#pragma once
#include "SNFrameworkInternal.h"


// 音楽リソースIDを定義する

enum SNMusicResID
{
	SNMusicResTop,

	SNMusicResDummy = SNMusicResTop,

	SNMusicResEnd = SNMusicResDummy,

	SNMusicResNum,
};

class SNMusicResource
{
public:
	static constexpr String ResourceInfoTable[SNMusicResNum] =
	{
		(String)L"natsuyasuminotanken.mp3",

	};
};
