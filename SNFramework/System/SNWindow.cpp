#include "SNWindow.h"
#include "SNWindowsAPI.h"
#include "../Resource/Resource.h"
#include "SNSystem.h"
#include "../Application/SNApplication.h"


// ウインドウクラス

// ウインドウプロシージャ
Void* __stdcall SNWindow::WindowProc(
    Void* window_handle,
    UInt message,
    Void* w_param,
    Void* l_param)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
    void* ret = 0;

    switch (message)
    {
    // 終了通知
    case WM_SNFRAMEWORK_NOTICE_EXIT:

        // 終了許可の場合
        if (static_cast<Boolean>(reinterpret_cast<intptr_t>(w_param)))
        {
            // ウインドウ破棄命令
            DestroyWindow((HWND)window_handle);
        }

        // 終了不可の場合
        else
        {
            // 要求をキャンセルする
            SNApplication::GetInstance()->RequestExitApplication(false);
        }

        break;

    // キーのアップダウンイベント
    case WM_KEYDOWN:
    case WM_KEYUP:
        // 何もしない
        // 余計な処理はしてほしくないのでイベントはひっかける
        break;

    // ウインドウクローズイベント
    case WM_CLOSE:

        // アプリケーションスレッドに対して終了要求
        SNApplication::GetInstance()->RequestExitApplication(true);
        break;

    // 描画イベント
    case WM_PAINT:
        // 描画開始
        hdc = BeginPaint((HWND)window_handle, &ps);

        // 描画コード
        GetClientRect((HWND)window_handle, &rect);

        // 黒画描画
        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdc, 0, 0, BLACKNESS);

        // 描画終了
        EndPaint((HWND)window_handle, &ps);
        break;

    // バックグラウンド消去
    case WM_ERASEBKGND:
        // 0を返すとウインドウ側で消去処理を実行
        // 0以外を返すとウインドウ側での消去処理はされない
        // 自前コードで消去処理を行うなら0以外を返すのが良い

        // SNFrameworkでは毎周期再描画を行うのでバックグラウンド消去処理はせずに
        // 0以外の値を返すことで余計な消去処理を動作させないようにする
        return (void*)1;

        // ウインドウアクティベイト
    //    case WM_ACTIVATE:
    //        Window::m_Active = (((DWORD)wParam & 0x0000FFFF) != WA_INACTIVE);
    //        (void*)DefWindowProc((HWND)hWnd, message, (WPARAM)wParam, (LPARAM)lParam);
    //        break;

    // ウインドウ破棄
    case WM_DESTROY:
        // 終了メッセージ発行
        PostQuitMessage(0);
        break;

    // その他イベント
    default:
        // デフォルトのプロシージャ実行
        return (void*)DefWindowProc((HWND)window_handle, message, (WPARAM)w_param, (LPARAM)l_param);
    }

    return 0;
}



// コンストラクタ
SNWindow::SNWindow()
{
    // 変数初期化
    WindowHandle = nullptr;

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

            wcex.style = CS_HREDRAW | CS_VREDRAW;
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
            RECT client_rect;

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
    }

    return;
}

// ウインドウハンドル取得
Handle SNWindow::GetWindowHandle()
{
    // ウインドウハンドルを返す
    return WindowHandle;
}

