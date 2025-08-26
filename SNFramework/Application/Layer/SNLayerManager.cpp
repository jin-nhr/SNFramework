#include "SNLayerManager.h"


// レイヤ管理クラス

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
SNTransitionCode SNLayerManager::Step(SNEvent* event)
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < LayerNum; loop_cnt++)
	{
		LayerList[loop_cnt]->Step(event);
	}

	// リターンコードを還す
	return SNTransitionCodeNo;
}

// 描画処理
Void SNLayerManager::Draw(SNSurface* surface)
{
	Int32 loop_cnt;

	// レイヤ登録の逆順に描画することで
	// 最初のレイヤが一番上、最後のレイヤが一番下になるようにする
	for (loop_cnt = LayerNum - 1; loop_cnt >= 0; loop_cnt--)
	{
		LayerList[loop_cnt]->Draw(surface);
	}

	return;
}

// レイヤ情報設定
Void SNLayerManager::SetLayerInfo(UInt8 layer_num, SNState** layer_list)
{
	// パラメータをメンバ変数に保持する
	LayerNum = layer_num;
	LayerList = layer_list;

	return;
}
