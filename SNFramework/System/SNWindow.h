#pragma once
#include "../Include/SNFramework.h"

// ウインドウクラス
class SNWindow
{
// 共有メソッド
public:
	// ウインドウ生成
	static Void Create(Handle application_incetance, Int32 show_cmd);

	// ウインドウプロシージャ
	static Void* __stdcall WindowProc(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	static Boolean EnableUpdate; // 画面更新フラグ
	static Handle WindowHandle;	// ウインドウハンドル
	static Handle WindowDC; // ウインドウDC
};
