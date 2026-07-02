#include "SNList.h"
#include "SNMath.h"
#include "SNWindowsAPI.h"


// リストクラス

// コンストラクタ
SNList::SNList()
{
	// 変数初期化
	ListArray = nullptr;
	ListNum = 0;
	AllocNum = 0;
	Top = nullptr;
	Last = nullptr;
	Pool = nullptr;
	return;
}

// デストラクタ
SNList::~SNList()
{
	ListArrayMemory.Free();
	return;
}

// メモリ確保
Void SNList::Allocate(Int32 num)
{
	Int32 cnt;

	// メモリ確保数の最低保証
	AllocNum = (Int32)SNMath::SelectMax(num, ListMinSize);
	
	// リストのメモリ確保
	ListArrayMemory.Allocate(sizeof(SNListContainer) * AllocNum);

	// リストのアドレス取得
	ListArray = (SNListContainer*)ListArrayMemory.GetAddress();

	// リスト数初期化
	ListNum = 0;

	// コンテナの初期化
	// 一度、先頭から終端までリンクしておく
	ListArray[0].Prev = nullptr;
	ListArray[0].UserData = 0;
	for (cnt = 1; cnt < AllocNum; cnt++)
	{
		ListArray[cnt - 1].Next = &ListArray[cnt];
		ListArray[cnt].Prev = &ListArray[cnt - 1];
		ListArray[cnt].UserData = 0;
	}
	ListArray[cnt - 1].Next = nullptr;

	// 先頭、最終は無効
	Top = nullptr;
	Last = nullptr;

	// プールの設定
	Pool = &ListArray[0];

	return;
}

// メモリ解放
Void SNList::Free()
{
	// 変数初期化
	ListArray = nullptr;
	ListNum = 0;
	AllocNum = 0;
	Top = nullptr;
	Last = nullptr;
	Pool = nullptr;

	// メモリ解放
	ListArrayMemory.Free();

	return;
}

// リスト解放 (全要素をPoolに戻す)
Void SNList::Clear()
{
	// Top～Lastの連続したリストをそのままPoolにリンクし
	// Top～Last～元のPoolの並びにする

	// 登録ありの場合
	// (Top/ Lastが有効であることが保証される)
	if (0 < ListNum)
	{
		// Poolに空きがある場合
		if (Pool != nullptr)
		{
			// Lastの後ろにPoolをリンク
			Pool->Prev = Last;
			Last->Next = Pool;
		}

		// Top以降をまるごとPoolに移動
		Pool = Top;
		Top = nullptr;
		Last = nullptr;

		// リスト数更新
		ListNum = 0;
	}

	return;
}

// リストア
// リストを再構築しリストの並び順と配列の並びを一致させる
Void SNList::Restore()
{
	SNList work_list;
	SNListContainer* it;

	// ワークのリストを確保
	work_list.Allocate(ListNum);

	// 本体の先頭
	it = Top;

	// ワークリストにデータを移す
	while (it != nullptr)
	{
		// ワークの後ろに要素追加しデータコピー
		work_list.InsertLast()->UserData = it->UserData;

		// 次に進める
		it = it->Next;
	}

	// リアロケート
	Allocate(ListNum);

	// ワークの先頭
	it = work_list.Top;

	// 本体側にデータを戻す
	while (it != nullptr)
	{
		// 本体の後ろに要素追加しデータコピー
		InsertLast()->UserData = it->UserData;

		// 次に進める
		it = it->Next;
	}

	return;
}

// 直接アクセス
// リストの配列にIndexで直接アクセスする
// リストア後の使用を想定
SNListContainer* SNList::DirectAccess(Int32 index)
{
	return &ListArray[index];
}

// リスト先頭追加
SNListContainer* SNList::InsertTop()
{
	// 先頭への追加
	return InsertAtIndex(0);
}

// リスト終端追加
SNListContainer* SNList::InsertLast()
{
	// 終端+1への追加
	return InsertAtIndex(ListNum);
}

// リスト追加(インデックス指定)
SNListContainer* SNList::InsertAtIndex(Int32 index)
{
	SNListContainer* ret = nullptr;
	SNListContainer* idx_container;

	// プールがあるときだけ処理
	if (Pool != nullptr)
	{
		// リスト登録数=0：最初の1個目登録
		if (ListNum <= 0)
		{
			// プールの要素をチェーンから取り出す
			ret = GetFromPool();

			// プールからコンテナが取れた
			if (ret != nullptr)
			{
				// データ初期化
				ret->UserData = 0;
				ret->Prev = nullptr;
				ret->Next = nullptr;
				Top = ret;
				Last = ret;
			}
		}

		// 指定Indexが0以下：Topへの追加
		else if (index <= 0)
		{
			// Topの手前に追加
			ret = InsertToBefore(Top);
		}

		// 有効Index範囲内：指定Index手前への追加
		else if (index < ListNum)
		{
			// Index画さすコンテナを取得
			// Indexが有効範囲内であることが保証されているので有効値が返る
			idx_container = SearchAtIndex(index);

			// 指定index手前への追加
			ret = InsertToBefore(idx_container);
		}

		// Index範囲外
		else
		{
			// Lastの次に追加
			ret = InsertToAfter(Last);
		}
	}

	return ret;
}

// リスト追加(指定コンテナの手前)
SNListContainer* SNList::InsertToBefore(SNListContainer* ref_container)
{
	SNListContainer* ret = nullptr;

	// 指定コンテナ有効時
	if (ref_container != nullptr)
	{
		// プールがあるときだけ処理
		if (Pool != nullptr)
		{
			// プールの要素をチェーンから取り出す
			ret = GetFromPool();

			// プールからコンテナが取れた
			if (ret != nullptr)
			{
				// 追加コンテナの設定
				ret->Next = ref_container;
				ret->Prev = ref_container->Prev;
				ret->UserData = 0;

				// 指定コンテナのリンク更新
				ref_container->Prev = ret;

				// 手前コンテナのリンク更新
				if (ret->Prev != nullptr)
				{
					ret->Prev->Next = ret;
				}

				// 指定コンテナがTopだったらTopを更新
				if (Top == ref_container)
				{
					Top = ret;
				}
			}
		}
	}

	return ret;
}

// リスト追加(指定コンテナの後ろ)
SNListContainer* SNList::InsertToAfter(SNListContainer* ref_container)
{
	SNListContainer* ret = nullptr;

	// 指定コンテナ有効時
	if (ref_container != nullptr)
	{
		// プールがあるときだけ処理
		if (Pool != nullptr)
		{
			// プールの要素をチェーンから取り出す
			ret = GetFromPool();

			// プールからコンテナが取れた
			if (ret != nullptr)
			{
				// 追加コンテナの設定
				ret->Next = ref_container->Next;
				ret->Prev = ref_container;
				ret->UserData = 0;

				// 指定コンテナのリンク更新
				ref_container->Next = ret;

				// 次コンテナのリンク更新
				if (ret->Next != nullptr)
				{
					ret->Next->Prev = ret;
				}

				// 指定コンテナがLastだったらLastを更新
				if (Last == ref_container)
				{
					Last = ret;
				}
			}
		}
	}

	return ret;
}


// リスト先頭取得
SNListContainer* SNList::GetTop()
{
	return Top;
}

// リスト終端取得
SNListContainer* SNList::GetLast()
{
	return Last;
}

// リスト取得(インデックス指定)
SNListContainer* SNList::GetAtIndex(Int32 index)
{
	return SearchAtIndex(index);
}

// リスト先頭削除
Void SNList::RemoveTop()
{
	// 先頭削除
	RemoveAtIndex(0);

	return;
}

// リスト終端削除
Void SNList::RemoveLast()
{
	// 終端削除
	RemoveAtIndex(ListNum - 1);

	return;
}

// リスト削除(インデックス指定)
Void SNList::RemoveAtIndex(Int32 index)
{
	SNListContainer* idx_container;

	// 登録なし
	if (ListNum <= 0)
	{
		// 何もしない
	}

	// Indexが範囲外
	else if ((index < 0) || (ListNum <= index))
	{
		// 何もしない
	}

	// Index有効
	else
	{
		// 指定Indexのコンテナ取得(このパスではnullptrになることはない)
		idx_container = SearchAtIndex(index);

		// コンテナ指定で削除
		Remove(idx_container);
	}

	return;
}

// リスト削除(コンテナ指定)
Void SNList::Remove(SNListContainer* ref_container)
{
	// 登録あり
	if (0 < ListNum)
	{
		// 指定コンテナ有効
		if (ref_container != nullptr)
		{
			// 最後の1個のコンテナではPrev/Nextがともにnullptrとなり
			// 削除後はTop/Lastはともにnullptrになる

			// 手前有効
			if (ref_container->Prev != nullptr)
			{
				// 手前と次をつなぐ
				ref_container->Prev->Next = ref_container->Next;
			}
			// 手前無効 = Top
			else
			{
				// 次をTopにする
				Top = ref_container->Next;
			}

			// 次有効
			if (ref_container->Next != nullptr)
			{
				// 次と手前をつなぐ
				ref_container->Next->Prev = ref_container->Prev;
			}

			// 次無効 = Last
			else
			{
				// 手前をLastにする
				Last = ref_container->Prev;
			}

			// Poolに戻す
			ReturnToPool(ref_container);
		}
	}
}

// 移動(インデックス指定)
Void SNList::MoveAtIndex(Int32 to_index, Int32 from_index)
{
	// from_indexのコンテナをto_indexに挿入する
	// to_indexがLastより後ろの場合、Lastの後ろに挿入する

	Int32 to_index_work;
	SNListContainer* to_container;
	SNListContainer* from_container;

	// to_Indexを0～ListNum(最大Index+1)の範囲に補正
	to_index_work = (Int32)SNMath::SelectMax(0, to_index);
	to_index_work = (Int32)SNMath::SelectMin(to_index_work, ListNum);

	// 移動元のIndexの範囲確認
	if ((from_index < 0) || (ListNum <= from_index))
	{
		// 移動元が範囲外のときは何もしない
	}

	// 移動元、先が同一Indexか確認
	else if (from_index == to_index_work)
	{
		// 移動元、先が同一のときは何もしない
	}

	// 移動元+1と移動先が同一か確認
	else if ((from_index + 1) == to_index_work)
	{
		// 移動元+1と移動先が同一の場合も実質移動不要のため何もしない
	}

	// 移動必要なケース
	else
	{
		// コンテナ取得
		to_container = SearchAtIndex(to_index_work);
		from_container = SearchAtIndex(from_index);

		// 移動先無効時 = 終端への移動
		if (to_container == nullptr)
		{
			// 終端の後ろへの移動処理
			MoveToAfter(Last, from_container);
		}

		// 移動先有効かつ移動元有効(移動元は範囲補正してるので必ず有効)
		else
		{
			// 移動先の手前への移動処理
			MoveToBefore(to_container, from_container);
		}
	}

	return;
}

// 移動(指定コンテナの手前)
Void SNList::MoveToBefore(SNListContainer* ref_container, SNListContainer* src_container)
{
	Void* user_data_bk;

	// 入力のコンテナが無効、または同一
	if ((ref_container == nullptr) || (src_container == nullptr) || (ref_container == src_container))
	{
		// 何もしない
	}
	else
	{
		// ユーザーデータのバックアップ
		user_data_bk = src_container->UserData;

		// 削除→追加を行っているがPoolはLIFO方式のため
		// 処理前後のsrcの一致性は担保される

		// srcを削除
		Remove(src_container);

		// ref手前に追加しデータ書き戻し
		InsertToBefore(ref_container)->UserData = user_data_bk;
	}

	return;
}

// 移動(指定コンテナの後ろ)
Void SNList::MoveToAfter(SNListContainer* ref_container, SNListContainer* src_container)
{
	Void* user_data_bk;

	// 入力のコンテナが無効、または同一
	if ((ref_container == nullptr) || (src_container == nullptr) || (ref_container == src_container))
	{
		// 何もしない
	}
	else
	{
		// ユーザーデータのバックアップ
		user_data_bk = src_container->UserData;

		// 削除→追加を行っているがPoolはLIFO方式のため
		// 処理前後のsrcの一致性は担保される

		// srcを削除
		Remove(src_container);

		// refの次に追加しｋデータ書き戻し
		InsertToAfter(ref_container)->UserData = user_data_bk;
	}

	return;
}

// 登録数取得
Int32 SNList::GetNum()
{
	return ListNum;
}

// プールから取得
SNListContainer* SNList::GetFromPool()
{
	SNListContainer* ret = nullptr;
	
	// プールあり
	if (Pool != nullptr)
	{
		// プールから切り出し
		ret = Pool;
		Pool = ret->Next;

		// コンテナ初期化
		ret->Next = nullptr;
		ret->Prev = nullptr;
		ret->UserData = 0;

		// Poolが有効
		if (Pool != nullptr)
		{
			// 取得したコンテナとのリンク削除
			Pool->Prev = nullptr;
		}

		// 登録数更新
		ListNum++;
	}

	return ret;
}

// プールに戻す
Void SNList::ReturnToPool(SNListContainer* container)
{
	// コンテナをPool先頭に設定
	container->Prev = nullptr;
	container->Next = Pool;
	container->UserData = 0;

	// Poolが有効
	if (Pool != nullptr)
	{
		// 元のPool先頭とのリンク
		Pool->Prev = container;
	}

	// Poolを更新
	Pool = container;

	// 登録数更新
	ListNum--;

	return;
}


// 指定Indexのコンテナ検索
SNListContainer* SNList::SearchAtIndex(Int32 index)
{
	SNListContainer* ret = nullptr;
	Int32 cnt;

	// indexが有効範囲外
	// ListNum=0のときもこの条件を満たす
	if ((index < 0) || (ListNum <= index))
	{
		ret = nullptr;
	}

	// 検索方向を判定
	// Indexが前半のときは先頭から走査
	else if (index <= (ListNum / 2))
	{
		ret = Top;

		// 先頭から指定Indexまで進める
		for (cnt = 0; cnt < index; cnt++)
		{
			ret = ret->Next;
		}
	}

	// Indexが後半のときは終端から走査
	else
	{
		ret = Last;

		// 終端から指定Indexまで戻す
		for (cnt = ((ListNum - 1) - index); 0 < cnt; cnt--)
		{
			ret = ret->Prev;
		}
	}

	return ret;
}

