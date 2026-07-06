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
	static Void* __stdcall WindowProc(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	static Handle WindowHandle;	// ウインドウハンドル
	static Handle WindowDC; // ウインドウDC

private:
	// フレームワーク終了通知
	static Void* OnNoticeExit(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウクローズ
	static Void* OnClose(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// セッション終了確認
	static Void* OnQueryEndSession(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// 背景消去
	static Void* OnEraseBackground(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウアクティベイト
	static Void* OnActivate(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// マウスホイール
	static Void* OnMouseWheel(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// ウインドウ破棄
	static Void* OnDestroy(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// サイズ変更
	static Void* OnSize(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// システムキー押下
	static Void* OnSysKeyDown(Void* window_handle, UInt message, Void* w_param, Void* l_param);

	// その他イベント処理
	static Void* OnOtherEvent(Void* window_handle, UInt message, Void* w_param, Void* l_param);
};
