#include "SNLayerManager.h"


// 状態管理クラス

// コンストラクタ
SNLayerManager::SNLayerManager()
{
	// 変数初期化
	LayerNum = 0;
	LayerList = nullptr;

	return;
}

// デストラクタ
SNLayerManager::~SNLayerManager()
{
	return;
}

// 初期化
Void SNLayerManager::Initialize()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Initialize();
	}

	return;
}

// 終了処理
Void SNLayerManager::Terminate()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Terminate();
	}

	return;
}

// Entry
Void SNLayerManager::Entry()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Entry();
	}

	return;
}

// Exit
Void SNLayerManager::Exit()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Exit();
	}

	return;
}

// 1フレーム実行
// リターン：遷移先コード -1:遷移なし
SNState::TransitionCode SNLayerManager::Step()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Step();
	}

	// リターンコードを還す
	return SNState::TransitionCode::NoTransition;
}

// レイヤ情報設定
Void SNLayerManager::SetLayerInfo(UInt8 layer_num, SNState** layer_list)
{
	// パラメータをメンバ変数に保持する
	LayerNum = layer_num;
	LayerList = layer_list;

	return;
}
