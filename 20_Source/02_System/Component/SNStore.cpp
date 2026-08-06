#include "SNStore.h"

// コンストラクタ
SNStore::SNStore()
{
	CreateResourceFunc = nullptr;
	DeleteResourceFunc = nullptr;

	return;
}

// デストラクタ
SNStore::~SNStore()
{
	DeleteStore();

	return;
}

Void SNStore::CreateStore(Int32 num)
{
	Int32 cnt;
	SNListContainer* it;

	DeleteStore();

	UsedList.Allocate(num);
	PoolList.Allocate(num);

	for (cnt = 0; cnt < num; cnt++)
	{
		it = PoolList.InsertLast();
		if (CreateResourceFunc != nullptr)
		{
			it->UserData = CreateResourceFunc();
		}
	}

	return;
}

Void SNStore::DeleteStore()
{
	SNListContainer* it;

	// Usedを強制的にPoolに移す
	while (UsedList.GetTop() != nullptr)
	{
		ListToList(&UsedList, &PoolList, UsedList.GetTop());
	}

	// Poolの全要素を解放する
	while (PoolList.GetTop() != nullptr)
	{
		it = PoolList.GetTop();
		if (DeleteResourceFunc != nullptr)
		{
			DeleteResourceFunc(it->UserData);
		}
		PoolList.Remove(it);
	}

	// メモリ解放
	UsedList.Clear();
	PoolList.Clear();

	return;
}

SNListContainer* SNStore::GetResource()
{
	SNListContainer* it;
	SNListContainer* to_it = nullptr;

	it = PoolList.GetTop();
	if (it != nullptr)
	{
		to_it = ListToList(&PoolList, &UsedList, it);
	}

	return to_it;
}

Void SNStore::ReleaseResource(SNListContainer* it)
{
	SNListContainer* to_it = nullptr;

	if (it != nullptr)
	{
		ListToList(&UsedList, &PoolList, it);
	}

	return;
}

SNListContainer* SNStore::ListToList(SNList* from, SNList* to, SNListContainer* it)
{
	SNListContainer* to_it;

	to_it = to->InsertTop();

	if (to_it != nullptr)
	{
		to_it->UserData = it->UserData;
	}

	from->Remove(it);

	return to_it;
}
