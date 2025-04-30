#include "SNSystem.h"
#include "SNWindowsAPI.h"


// プログラムのエントリーポイント
int APIENTRY wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow)
{
    // システムクラスのインスタンス取得
    SNSystem* sn_system = SNSystem::GetInstance();

    // システム初期化
    sn_system->Initialize((Handle)hInstance, (Handle)hPrevInstance, (String)lpCmdLine, (Int32)nCmdShow);

    // システム起動準備
    sn_system->Startup();

    // システム実行
    sn_system->Run();

    // システム終了前処理
    sn_system->BeforeTerminate();

    // システム終了
    sn_system->Terminate();

    // インスタンス破棄
    SNSystem::Destroy();

    return 0;
}
