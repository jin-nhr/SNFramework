#include "SNSysApp.h"


SNSysApp::SNSysApp()
{
	SetSceneNum(14);
	// TransCode        0   1   2   3
	SetScene(&Startup,  1, -1, -1, -1);	// 0 スタートアップ
	SetScene(&Loading,  3, -1, -1,  2);	// 1 ローディング
	SetScene(&Error,   -1, -1, -1, -1);	// 2 エラー終了
	SetScene(&Title,    9, -1,  6,  5); // 3 オープニング
	SetScene(&Title,    9, -1, -1,  5);	// 4 仮：ロード画面
	SetScene(&PreClose, 3, -1, -1, -1); // 5 終了確認(オープニング)
	SetScene(&Config,   3, -1, -1,  5);	// 6 システム設定(オープニング)
	SetScene(&Config,   6, -1, -1,  5);	// 7 仮：ゲームパッド割り当て(オープニング)
	SetScene(&Config,   6, -1, -1,  5);	// 8 仮：キーボード割り当て(オープニング)
	SetScene(&Idle,    11, -1, -1, 10);	// 9 アイドル
	SetScene(&PreClose, 9, -1, -1, -1); // 10 終了確認(アイドル)
	SetScene(&Config,   9, -1, -1, 10);	// 11 システム設定(アイドル)
	SetScene(&Config,  11, -1, -1, 10);	// 12 仮：ゲームパッド割り当て(アイドル)
	SetScene(&Config,  11, -1, -1, 10);	// 13 仮：キーボード割り当て(アイドル)

	return;
}

SNSysApp::~SNSysApp()
{
	return;
}
