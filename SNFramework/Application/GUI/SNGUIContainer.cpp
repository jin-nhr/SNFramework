#include "SNGUIContainer.h"


// レイヤ管理クラス

// コンストラクタ
SNGUIContainer::SNGUIContainer()
{
	// 変数初期化
	GUIPartsNum = 0;
	GUIPartsList = nullptr;

	return;
}

// デストラクタ
SNGUIContainer::~SNGUIContainer()
{
	return;
}

// 初期化
Void SNGUIContainer::Initialize()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < GUIPartsNum; loop_cnt++)
	{
		GUIPartsList[loop_cnt]->Initialize();
	}

	return;
}

// 終了処理
Void SNGUIContainer::Terminate()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < GUIPartsNum; loop_cnt++)
	{
		GUIPartsList[loop_cnt]->Terminate();
	}

	return;
}

// Entry
Void SNGUIContainer::Entry()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < GUIPartsNum; loop_cnt++)
	{
		GUIPartsList[loop_cnt]->Entry();
	}

	return;
}

// Exit
Void SNGUIContainer::Exit()
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < GUIPartsNum; loop_cnt++)
	{
		GUIPartsList[loop_cnt]->Exit();
	}

	return;
}

// 1フレーム実行
// リターン：遷移先コード -1:遷移なし
SNTransitionCode SNGUIContainer::Step(SNEvent* event)
{
	Int32 loop_cnt;

	for (loop_cnt = 0; loop_cnt < GUIPartsNum; loop_cnt++)
	{
		GUIPartsList[loop_cnt]->Step(event);
	}

	// リターンコードを還す
	return SNTransitionCodeNo;
}

// 描画処理
Void SNGUIContainer::Draw(SNSurface* surface)
{
	Int32 loop_cnt;

	// レイヤ登録の逆順に描画することで
	// 最初のレイヤが一番上、最後のレイヤが一番下になるようにする
	for (loop_cnt = GUIPartsNum - 1; loop_cnt >= 0; loop_cnt--)
	{
		GUIPartsList[loop_cnt]->Draw(surface);
	}

	return;
}

// 座標設定
Void SNGUIContainer::SetPosition(Int32 x, Int32 y)
{
	SNGUIBase::SetPosition(x, y);
	UpdateChildRect();

	return;
}

// サイズ設定
Void SNGUIContainer::SetSize(Int32 w, Int32 h)
{
	SNGUIBase::SetSize(w, h);
	UpdateChildRect();

	return;
}

// 親座標設定
Void SNGUIContainer::SetParentPosition(Int32 x, Int32 y)
{
	SNGUIBase::SetParentPosition(x, y);
	UpdateChildRect();

	return;
}

// 親サイズ設定
Void SNGUIContainer::SetParentSize(Int32 w, Int32 h)
{
	SNGUIBase::SetParentSize(w, h);
	UpdateChildRect();

	return;
}

// 子情報更新
Void SNGUIContainer::UpdateChildRect()
{
	Int32 loop_cnt;
	SNRect rect;

	// グローバル情報取得
	rect = GetGlobalRect();

	// 親情報設定
	for (loop_cnt = GUIPartsNum - 1; loop_cnt >= 0; loop_cnt--)
	{
		GUIPartsList[loop_cnt]->SetParentRect(rect.PointX, rect.PointY, rect.Width, rect.Height);
	}

	return;
}

// レイヤ情報設定
Void SNGUIContainer::SetGUIPartsInfo(UInt8 gui_num, SNGUIBase** gui_list)
{
	Int32 loop_cnt;
	SNRect rect;

	// パラメータをメンバ変数に保持する
	GUIPartsNum = gui_num;
	GUIPartsList = gui_list;

	// 自身のグローバル情報取得
	rect = GetGlobalRect();

	// 親情報設定
	for (loop_cnt = GUIPartsNum - 1; loop_cnt >= 0; loop_cnt--)
	{
		GUIPartsList[loop_cnt]->SetParentRect(rect.PointX, rect.PointY, rect.Width, rect.Height);
	}

	return;
}
