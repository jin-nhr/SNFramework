#pragma once
#include "SNFrameworkInternal.h"


// サウンドリソースIDを定義する

enum SNSoundResID
{
	SNSoundResTop,

	SNSoundResWindowOpen = SNSoundResTop,
	SNSoundResFocusMove,
	SNSoundResButtonPush,

	SNSoundResEnd = SNSoundResButtonPush,

	SNSoundResNum,
};

class SNSoundResource
{
public:
	static constexpr String ResourceInfoTable[SNSoundResNum] =
	{
		(String)L"ウインドウオープン.mp3",
		(String)L"カーソル移動.mp3",
		(String)L"ボタン押下.mp3",

	};
};
