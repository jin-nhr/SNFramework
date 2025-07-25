#pragma once
#include "../Include/SNFramework.h"

// ウインドウクラス
class SNWindow
{
// 共有メソッド
private:
	// ウインドウプロシージャ
	static Void* __stdcall WindowProc(Void* window_handle, UInt message, Void* w_param, Void* l_param);

public:
	// コンストラクタ
	SNWindow();

	// デストラクタ
	~SNWindow();

	// ウインドウ生成
	Void Create(Handle application_incetance, Int32 show_cmd);

	// ウインドウハンドル取得
	Handle GetWindowHandle();

	// クライアント領域DC取得
	Handle GetClientDC();

private:
	 Handle WindowHandle;	// ウインドウハンドル
	 Handle ClientDC;		// クライアント領域DC
};
