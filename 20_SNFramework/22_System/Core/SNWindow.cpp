#include "SNWindow.h"
#include "SNWindowsAPI.h"
#include "Resource.h"
#include "SNSystem.h"
#include "SNApplication.h"
#include "SNGraphics.h"
#include "SNConfig.h"
#include "SNGDI.h"

// ウインドウクラス

Boolean SNWindow::EnableUpdate = false;
Handle SNWindow::WindowHandle = nullptr;
Handle SNWindow::WindowDC = nullptr;

// ウインドウプロシージャ
Void* __stdcall SNWindow::WindowProc(
    Void* window_handle,
    UInt message,
    Void* w_param,
    Void* l_param)
{
    Void* ret = 0;

    switch (message)
    {
    // 終了通知
    case WM_SNFRAMEWORK_NOTICE_EXIT:
        ret = OnNoticeExit(window_handle, message, w_param, l_param);
        break;

    // ウインドウクローズイベント
    case WM_CLOSE:
        ret = OnClose(window_handle, message, w_param, l_param);
        break;

    // セッション終了確認
    case WM_QUERYENDSESSION:
        ret = OnQueryEndSession(window_handle, message, w_param, l_param);
        break;

    // 描画イベント
    case WM_PAINT:
        ret = OnPaint(window_handle, message, w_param, l_param);
        break;

    // バックグラウンド消去
    case WM_ERASEBKGND:
        ret = OnEraseBackground(window_handle, message, w_param, l_param);
        break;

    // ウインドウアクティベイト
    case WM_ACTIVATE:
        ret = OnActivate(window_handle, message, w_param, l_param);
        break;

    // マウスホイール
    case WM_MOUSEWHEEL:
        ret = OnMouseWheel(window_handle, message, w_param, l_param);
        break;

    // ウインドウ破棄
    case WM_DESTROY:
        ret = OnDestroy(window_handle, message, w_param, l_param);
        break;

    // ウインドウのサイズ変更
    case WM_SIZE:
        ret = OnSize(window_handle, message, w_param, l_param);
        break;

    // ALT系のキー押下
    case WM_SYSKEYDOWN:
        ret = OnSysKeyDown(window_handle, message, w_param, l_param);
        break;

    // その他イベント
    default:
        ret = OnOtherEvent(window_handle, message, w_param, l_param);
        break;
    }

    return ret;
}

// ウインドウ生成
// リターン：処理結果を返す true:正常
Void SNWindow::Create(Handle application_incetance, Int32 show_cmd)
{
    HDC dc;

    // ウインドウ未生成のときだけ処理
    if (WindowHandle == nullptr)
    {
        // Windowクラス登録
        {
            WNDCLASSEXW wcex;

            wcex.cbSize = sizeof(WNDCLASSEX);

            wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wcex.lpfnWndProc = (WNDPROC)WindowProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = (HINSTANCE)application_incetance;
            wcex.hIcon = LoadIcon((HINSTANCE)application_incetance, MAKEINTRESOURCE(IDI_ICON));
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wcex.lpszMenuName = NULL;
            wcex.lpszClassName = SNSystemConfig::ApplicationName,
            wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL_ICON));

            RegisterClassExW(&wcex);
        }

        // Window生成
        {
            // サイズ計算
            RECT win_rect;

            // 画面幅、高さからクライアント領域の座標を計算
            win_rect.left = 0;
            win_rect.right = SNSystemConfig::ScreenWidth - 1;
            win_rect.top = 0;
            win_rect.bottom = SNSystemConfig::ScreenHeight - 1;

            // クライアント領域の座標からウインドウ座標を取得
            AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, FALSE);


            // Window生成
            WindowHandle = (void*)CreateWindowW(
                SNSystemConfig::ApplicationName,
                SNSystemConfig::ApplicationName,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                0,
                win_rect.right - win_rect.left + 1,
                win_rect.bottom - win_rect.top + 1,
                nullptr,
                nullptr,
                (HINSTANCE)application_incetance,
                nullptr);

            // ウインドウ表示設定
            ShowWindow((HWND)WindowHandle, show_cmd);
            UpdateWindow((HWND)WindowHandle);
        }

        dc = ::GetDC((HWND)WindowHandle);

        // DCへの初期設定を行う
        {
            SNGDI gdi;

            gdi.InitDC(dc);
        }

        WindowDC = (Handle)dc;
    }

    return;
}

// 表示設定
Void SNWindow::SetWindowDisp(Boolean fullscreen)
{
    // フルスクリーン設定
    if (fullscreen)
    {
        ShowWindow((HWND)WindowHandle, SW_MAXIMIZE);
    }

    // 通常設定
    else
    {
        ShowWindow((HWND)WindowHandle, SW_RESTORE);
    }

    return;
}

// フレームワーク終了通知
Void* SNWindow::OnNoticeExit(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // ウインドウ破棄命令
    DestroyWindow((HWND)window_handle);

    return 0;
}

// ウインドウクローズ
Void* SNWindow::OnClose(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // アプリケーションスレッドに対して終了通知
    SNApplication::NotifyEvent(SNApplicationEvent::SNEventExitApplication);

    return 0;
}

// セッション終了確認
Void* SNWindow::OnQueryEndSession(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // アプリケーションスレッドに対して終了通知
    SNApplication::NotifyEvent(SNApplicationEvent::SNEventExitApplication);

    // Windowsに対しては終了を拒否し、アプリケーション独自に終了処理を実行する
    // 拒否した場合でもWindowsからプロセスをキルされる可能性はある
    return (Void*)FALSE;
}

// 描画更新
Void* SNWindow::OnPaint(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    RECT rect;
    PAINTSTRUCT ps;

    BeginPaint((HWND)window_handle, &ps);
    {
        // クライアント領域サイズを取得
        GetClientRect((HWND)window_handle, &rect);

        if (EnableUpdate)
        {
            // 画面描画処理
            SNGraphics::DrawScreen((Handle)ps.hdc, rect.right, rect.bottom);
        }

        // ウインドウ生成直後にちらつくのを防止
        else
        {
            SNGDI gdi;
            // 黒で塗りつぶし
            gdi.PatBlt(ps.hdc, 0, 0, rect.right, rect.bottom, BLACKNESS);
        }
    }
    EndPaint((HWND)window_handle, &ps);

    return 0;
}

// 背景消去
Void* SNWindow::OnEraseBackground(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // 0を返すとウインドウ側で消去処理を実行
    // 0以外を返すとウインドウ側での消去処理はされない
    // 自前コードで消去処理を行うなら0以外を返すのが良い

    // SNFrameworkでは毎周期再描画を行うのでバックグラウンド消去処理はせずに
    // 0以外の値を返すことで余計な消去処理を動作させないようにする
    return (Void*)1;
}

// ウインドウアクティベイト
Void* SNWindow::OnActivate(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // アクティブならtrue, 非アクティブならfalse
    if (static_cast<DWORD>(reinterpret_cast<intptr_t>(w_param) & 0x0000FFFF) != WA_INACTIVE)
    {
        // アクティブ通知
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventActive);
    }
    else
    {
        // 非アクティブ通知
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventNonActive);
    }

    // ウインドウデフォルト処理
    (void*)DefWindowProc((HWND)window_handle, message, (WPARAM)w_param, (LPARAM)l_param);
    return 0;
}

// マウスホイール
Void* SNWindow::OnMouseWheel(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    Int32 wheel_updown;

    // 回転量を取得
    wheel_updown = GET_WHEEL_DELTA_WPARAM(w_param);

    // 正ならUp
    if (wheel_updown > 0)
    {
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventWheelUp);
    }
    // 負ならDown
    else if (wheel_updown < 0)
    {
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventWheelDown);
    }
    else
    {
        // 0のときは通知なし
    }

    return 0;
}

// ウインドウ破棄
Void* SNWindow::OnDestroy(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // 終了メッセージ発行
    PostQuitMessage(0);

    return 0;
}

// サイズ変更
Void* SNWindow::OnSize(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    RECT rect;
    LONG style;
    LONG exstyle;

    // ウインドウスタイル取得
    style = GetWindowLong((HWND)window_handle, GWL_STYLE);
    exstyle = GetWindowLong((HWND)window_handle, GWL_EXSTYLE);

    // 最大化時 (最前面＋フレームなし)
    if ((WPARAM)w_param == SIZE_MAXIMIZED)
    {
        // タイトルバーとフレームなし設定
        style &= ~(WS_CAPTION | WS_THICKFRAME);
        SetWindowLong((HWND)window_handle, GWL_STYLE, style);

        // フレームなし設定
        exstyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong((HWND)window_handle, GWL_EXSTYLE, exstyle);

        // 画面サイズ取得
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

        // ウインドウサイズを画面サイズに合わせて調整
        // Windowsの謎の動きによりサイズ調整をしないと見切れが発生してしまう
        SetWindowPos((HWND)window_handle, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED);

        // アプリケーションに状態を通知
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventMaximizeDisp);
    }

    // 最大化解除時 (最前面解除＋フレームあり)
    else if ((WPARAM)w_param == SIZE_RESTORED)
    {
        // フレーム設定
        style |= (WS_CAPTION | WS_THICKFRAME);
        SetWindowLong((HWND)window_handle, GWL_STYLE, style);

        // TOPMOST解除とフレーム設定
        exstyle = GetWindowLong((HWND)window_handle, GWL_EXSTYLE);
        exstyle &= ~WS_EX_TOPMOST;
        exstyle |= (WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
        SetWindowLong((HWND)window_handle, GWL_EXSTYLE, exstyle);

        // ウインドウの外観を更新し、TOPMOST解除
        SetWindowPos((HWND)window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

        // アプリケーションに状態を通知
        SNApplication::NotifyEvent(SNApplicationEvent::SNEventNormalDisp);
    }
    else
    {
        // 上記以外は処理なし
    }

    return 0;
}

// システムキー押下
Void* SNWindow::OnSysKeyDown(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    WINDOWPLACEMENT wp;

    // ALT+ENTER が押された
    if (((WPARAM)w_param == VK_RETURN) && (GetKeyState(VK_MENU) & 0x8000))
    {
        wp = { sizeof(WINDOWPLACEMENT) };
        GetWindowPlacement((HWND)window_handle, &wp);

        // 最大化表示中→解除
        if (wp.showCmd == SW_SHOWMAXIMIZED)
        {
            ShowWindow((HWND)window_handle, SW_RESTORE);
        }
        // 通常表示中→最大化
        else if (wp.showCmd == SW_SHOWNORMAL)
        {
            ShowWindow((HWND)window_handle, SW_MAXIMIZE);
        }
    }

    return 0;
}

// その他イベント処理
Void* SNWindow::OnOtherEvent(Void* window_handle, UInt message, Void* w_param, Void* l_param)
{
    // デフォルトのプロシージャ実行
    return (Void*)DefWindowProc((HWND)window_handle, message, (WPARAM)w_param, (LPARAM)l_param);
}
