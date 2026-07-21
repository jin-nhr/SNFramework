#include "SNSystem.h"
#include "SNWindowsAPI.h"
#include "SNError.h"

// エントリーポイント
// システムクラスに対して初期化、起動準備、実行、終了前処理、終了処理を順に実行する
int APIENTRY wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow)
{
    try
    {
        try
        {
            // システム初期化
            SNSystem::Initialize((Handle)hInstance, (Handle)hPrevInstance, (String)lpCmdLine, (Int32)nCmdShow);

            // システム起動準備
            SNSystem::Startup();

            // システム実行
            SNSystem::Run();

            // システム終了前処理
            SNSystem::BeforeTerminate();

            // システム終了
            SNSystem::Terminate();
        }
        catch (...)
        {
            SNError::ErrorCode = SNErrorPrimaryException;

            // システム終了前処理
            SNSystem::BeforeTerminate();

            // システム終了
            SNSystem::Terminate();
        }
    }
    catch (...)
    {
        // 終了処理で例外検知の場合は終了処理をあきらめる
        SNError::ErrorCode = SNErrorTerminateException;
    }

    // エラー検知していればエラーログ出力
    SNError::ErrorLogOut();

    return 0;
}
