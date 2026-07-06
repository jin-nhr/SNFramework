#pragma once

// Windows関連のヘッダインクルードおよび定義を行うファイル

/////////////////////////////////////////////////////////////////
// ヘッダインクルード

// OSおよび開発環境で必要なヘッダ
#include "SNTargetVersion.h"
#define WIN32_LEAN_AND_MEAN

// ウインドウズ、Cランタイム
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <strsafe.h>
#include <process.h>
#include <timeapi.h>
#include <mmsystem.h>
#include <shlwapi.h>
#include <gdiplus.h>
#include <wincodec.h>

// DirectX関連
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

/////////////////////////////////////////////////////////////////
// ライブラリリンクの追加

// ウインドウズ、Cランタイムのライブラリ
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

// DirectX関連
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

/////////////////////////////////////////////////////////////////
// Windowsメッセージの追加

// フレームワーク固有のWindowsメッセージ
enum
{
	WM_SNFRAMEWORK_NOTICE_EXIT = WM_APP,		// 終了応答
};
