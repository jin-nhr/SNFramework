#pragma once
#include "SNFrameworkInternal.h"
#include "SNString.h"

// エラークラス
// エラー検知時に情報を残し、アプリ終了時にログファイルに出力する


enum SNErrorCode
{
	SNErrorNon,
	SNErrorPrimaryException,
	SNErrorThreadException,
	SNErrorTerminateException,
};

class SNError
{
public:
	static Void ErrorLogOut();
public:
	static SNErrorCode ErrorCode;
};

