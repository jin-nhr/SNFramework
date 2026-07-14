#pragma once
#include "SNFrameworkInternal.h"

// ウインドウクラス
// アプリケーションのメインウインドウの生成、制御を行う
// メインウインドウはプライマリスレッドに配置し、アプリケーションとの通信を行う

class SNWindow
{
// 共有メソッド
public:
	// ウインドウ生成
	static Void Create(Handle application_incetance, Int32 show_cmd);

	// ウインドウプロシージャ
	static Int64 __stdcall WindowProc(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	static Handle WindowHandle;	// ウインドウハンドル
	static Handle WindowDC; // ウインドウDC

private:
	// フレームワーク終了通知
	static Int64 OnNoticeExit(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウクローズ
	static Int64 OnClose(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// セッション終了確認
	static Int64 OnQueryEndSession(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// 背景消去
	static Int64 OnEraseBackground(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウアクティベイト
	static Int64 OnActivate(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// マウスホイール
	static Int64 OnMouseWheel(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウ破棄
	static Int64 OnDestroy(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// サイズ変更
	static Int64 OnSize(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// システムキー押下
	static Int64 OnSysKeyDown(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// その他イベント処理
	static Int64 OnOtherEvent(Void* window_handle, UInt message, Void* w_param, Void* l_param);
};
