#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"

// リストコンテナクラス
struct SNListContainer
{
	SNListContainer* Prev;	// 前要素
	SNListContainer* Next;	// 次要素
	Void* UserData;			// ユーザーデータ
};


// リストクラス
// 双方向リスト構造
// ユーザーデータ実体は使用側で管理すること
class SNList
{
public:
	// リスト最小サイズ
	static constexpr Int32 ListMinSize = 4;

public:
	// コンストラクタ
	SNList();

	// デストラクタ
	~SNList();

	// メモリ確保
	Void Allocate(Int32 num);

	// メモリ解放
	Void Free();

	// リスト解放 (全要素をPoolに戻す)
	Void Clear();

	// リストア
	// リストを再構築しリストの並び順と配列の並びを一致させる
	Void Restore();

	// 直接アクセス
	// リストの配列にIndexで直接アクセスする
	// リストア後の使用を想定
	SNListContainer* DirectAccess(Int32 index);

	// リスト先頭追加
	SNListContainer* InsertTop();

	// リスト終端追加
	SNListContainer* InsertLast();

	// リスト追加(インデックス指定)
	SNListContainer* InsertAtIndex(Int32 index);

	// リスト追加(指定コンテナの手前)
	SNListContainer* InsertToBefore(SNListContainer* ref_container);

	// リスト追加(指定コンテナの後ろ)
	SNListContainer* InsertToAfter(SNListContainer* ref_container);

	// リスト先頭取得
	SNListContainer* GetTop();

	// リスト終端取得
	SNListContainer* GetLast();

	// リスト取得(インデックス指定)
	SNListContainer* GetAtIndex(Int32 index);

	// リスト先頭削除
	Void RemoveTop();

	// リスト終端削除
	Void RemoveLast();

	// リスト削除(インデックス指定)
	Void RemoveAtIndex(Int32 index);

	// リスト削除(コンテナ指定)
	Void Remove(SNListContainer* ref_container);

	// 移動(インデックス指定)
	Void MoveAtIndex(Int32 to_index, Int32 from_index);

	// 移動(指定コンテナの手前)
	Void MoveToBefore(SNListContainer* ref_container, SNListContainer* src_container);

	// 移動(指定コンテナの後ろ)
	Void MoveToAfter(SNListContainer* ref_container, SNListContainer* src_container);

	// 登録数取得
	Int32 GetNum();

private:
	// プールから取得
	SNListContainer* GetFromPool();

	// プールに戻す
	Void ReturnToPool(SNListContainer* container);

	// 指定Indexのコンテナ検索
	SNListContainer* SearchAtIndex(Int32 index);

	SNMemory ListArrayMemory;		// リストコンテナメモリ
	SNListContainer* ListArray;		// リストコンテナ配列
	Int32 ListNum;					// リスト数
	Int32 AllocNum;					// メモリ確保数

	SNListContainer* Top;			// 先頭
	SNListContainer* Last;			// 終端
	SNListContainer* Pool;			// プール(空き要素のチェーン)
};
