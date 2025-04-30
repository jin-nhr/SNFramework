#pragma once

// OSおよび開発環境で必要なヘッダインクルード
#include "SNTargetVersion.h"
#define WIN32_LEAN_AND_MEAN

// ウインドウズ、Cランタイムインクルード
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <process.h>
#include <timeapi.h>

// ウインドウズ、Cランタイムのライブラリ
#pragma comment(lib, "Winmm.lib")



// フレームワーク固有のメッセージ
enum
{
	WM_SNFRAMEWORK_NOTICE_EXIT = WM_APP,		// 終了応答
};
