#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"



// ストアクラス
// 使用中リストとプールリストをもち
// リソースの管理を行う
class SNStore
{
public:
	// 使用前に以下メソッドの設定をしておくこと

	// リソース生成メソッド
	Void* (*CreateResourceFunc)();

	// リソース破棄メソッド
	Void(*DeleteResourceFunc)(Void* res);

public:
	// コンストラクタ
	SNStore();

	// デストラクタ
	virtual ~SNStore();

	virtual Void CreateStore(Int32 num);

	virtual Void DeleteStore();
	
	virtual SNListContainer* GetResource();

	virtual Void ReleaseResource(SNListContainer* it);

private:
	// fromのitをto側に移動する
	virtual SNListContainer* ListToList(SNList* from, SNList* to, SNListContainer* it);


	SNList UsedList;
	SNList PoolList;
};
