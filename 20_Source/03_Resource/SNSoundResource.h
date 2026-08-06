#pragma once
#include "SNFrameworkInternal.h"


// サウンドリソースIDを定義する

enum SNSoundResID
{
	SNSoundResTop,

	SNSoundResFocusMove = SNSoundResTop,
	SNSoundResButtonPush,

	SNSoundResEnd = SNSoundResButtonPush,

	SNSoundResNum,
};

class SNSoundResource
{
public:
	static constexpr String ResourceInfoTable[SNSoundResNum] =
	{
		(String)L"カーソル移動2.mp3",
		(String)L"決定ボタンを押す7.mp3",

	};
};
