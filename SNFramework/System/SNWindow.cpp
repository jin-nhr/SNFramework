#include "SNWindow.h"
#include "SNWindowsAPI.h"
#include "../Resource/Resource.h"
#include "SNSystem.h"
#include "../Application/SNApplication.h"
#include "../Graphics/SNGraphics.h"


// ウインドウクラス

Boolean SNWindow::EnableUpdate = false;

// ウインドウプロシージャ
Void* __stdcall SNWindow::WindowProc(
    Void* window_handle,
    UInt message,
    Void* w_param,
    Void* l_param)
{
    RECT rect;
    Void* ret = 0;
    Int32 wheel_updown;
    PAINTSTRUCT ps;

    switch (message)
    {
    // 終了通知
    case WM_SNFRAMEWORK_NOTICE_EXIT:
        // ウインドウ破棄命令
        DestroyWindow((HWND)window_handle);
        break;

    // キーのアップダウンイベント
    case WM_KEYDOWN:
    case WM_KEYUP:
        // 何もしない
        // 余計な処理はしてほしくないのでイベントはひっかける
        break;

    // ウインドウクローズイベント
    case WM_CLOSE:
        // アプリケーションスレッドに対して終了通知
        SNApplication::GetInstance()->NotifyExitApplication();
        break;

    // セッション終了確認
    case WM_QUERYENDSESSION:
        // アプリケーションスレッドに対して終了通知
        SNApplication::GetInstance()->NotifyExitApplication();

        // Windowsに対しては終了を拒否し、アプリケーション独自に終了処理を実行する
        // 拒否した場合でもWindowsからプロセスをキルされる可能性はある
        return (Void*)FALSE;

    // 描画イベント
    case WM_PAINT:
        BeginPaint((HWND)window_handle, &ps);
        if (EnableUpdate)
        {
            // クライアント領域サイズを取得
            GetClientRect((HWND)window_handle, &rect);

            // 画面描画処理
            SNGraphics::GetInstance()->DrawScreen((Handle)ps.hdc, rect.right, rect.bottom);
        }
        EndPaint((HWND)window_handle, &ps);

        break;

    // バックグラウンド消去
    case WM_ERASEBKGND:
        // 0を返すとウインドウ側で消去処理を実行
        // 0以外を返すとウインドウ側での消去処理はされない
        // 自前コードで消去処理を行うなら0以外を返すのが良い

        // SNFrameworkでは毎周期再描画を行うのでバックグラウンド消去処理はせずに
        // 0以外の値を返すことで余計な消去処理を動作させないようにする
        return (Void*)1;

        // ウインドウアクティベイト
    case WM_ACTIVATE:
        // アクティブならtrue, 非アクティブならfalse
        if (static_cast<DWORD>(reinterpret_cast<intptr_t>(w_param) & 0x0000FFFF) != WA_INACTIVE)
        {
            // アクティブ通知
            SNApplication::GetInstance()->NotifyActive();
        }
        else
        {
            // 非アクティブ通知
            SNApplication::GetInstance()->NotifyNonActive();
        }

        // ウインドウデフォルト処理
        (void*)DefWindowProc((HWND)window_handle, message, (WPARAM)w_param, (LPARAM)l_param);
        break;

    // マウスホイール
    case WM_MOUSEWHEEL:
        // 回転量を取得
        wheel_updown = GET_WHEEL_DELTA_WPARAM(w_param);

        // 正ならUp
        if (wheel_updown > 0)
        {
            SNApplication::GetInstance()->NotifyWheelUp();
        }
        // 負ならDown
        else if (wheel_updown < 0)
        {
            SNApplication::GetInstance()->NotifyWheelDown();
        }
        else
        {
            // 0のときは通知なし
        }
        break;

    // ウインドウ破棄
    case WM_DESTROY:
        // 終了メッセージ発行
        PostQuitMessage(0);
        break;

    // その他イベント
    default:
        // デフォルトのプロシージャ実行
        return (Void*)DefWindowProc((HWND)window_handle, message, (WPARAM)w_param, (LPARAM)l_param);
    }

    return 0;
}


// コンストラクタ
SNWindow::SNWindow()
{
    // 変数初期化
    WindowHandle = nullptr;
    ClientDC = nullptr;

    return;
}

// デストラクタ
SNWindow::~SNWindow()
{
    return;
}

// ウインドウ生成
// リターン：処理結果を返す true:正常
Void SNWindow::Create(Handle application_incetance, Int32 show_cmd)
{
    // ウインドウ未生成のときだけ処理
    if (WindowHandle == nullptr)
    {
        // コンフィギュレーションデータ取得
        SNConfigurationData* configuration_data = &SNConfiguration::GetInstance()->ConfigurationData;

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
            wcex.lpszClassName = configuration_data->System.ApplicationName;
            wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL_ICON));

            RegisterClassExW(&wcex);
        }

        // Window生成
        {
            // サイズ計算
            RECT win_rect;

            // 画面幅、高さからクライアント領域の座標を計算
            win_rect.left = 0;
            win_rect.right = configuration_data->System.ScreenWidth - 1;
            win_rect.top = 0;
            win_rect.bottom = configuration_data->System.ScreenHeight - 1;

            // クライアント領域の座標からウインドウ座標を取得
            AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, FALSE);


            // Window生成
            WindowHandle = (void*)CreateWindowW(
                configuration_data->System.ApplicationName,
                configuration_data->System.ApplicationName,
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

        // クライアント領域DCを取得
        ClientDC = (Handle)GetDC((HWND)WindowHandle);

        // DCへの初期設定を行う
        SetBkMode((HDC)ClientDC, TRANSPARENT);
        SetStretchBltMode((HDC)ClientDC, STRETCH_HALFTONE);
        SetBrushOrgEx((HDC)ClientDC, 0, 0, NULL);
    }

    return;
}

// ウインドウハンドル取得
Handle SNWindow::GetWindowHandle()
{
    // ウインドウハンドルを返す
    return WindowHandle;
}

// クライアント領域DC取得
Handle SNWindow::GetClientDC()
{
    // クライアント領域DCを返す
    return ClientDC;
}
