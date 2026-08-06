#pragma once
#include "SNFrameworkInternal.h"
#include "SNResourceIF.h"

// アンマネージリソースの確保/解放を自動で行うクラス
class SNAutoResource
{
public:
	// コンストラクタ
	SNAutoResource(SNResourceIF* resource);

	// デストラクタ
	virtual ~SNAutoResource();


private:
	SNResourceIF* Resource;	// リソース
};

