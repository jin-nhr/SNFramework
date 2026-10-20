#pragma once
#include "SNFrameworkInternal.h"


// グラフィックリソースIDを定義する

enum SNGraphicsResID
{
	// -----------------------------------------------
	// 起動リソース

	SNGraphicsResTop = 0,

	SNGraphicsResGUIAndWorld = SNGraphicsResTop,
	SNGraphicsResStartLogo1,
	SNGraphicsResStartLogo2,
	SNGraphicsResAppTitle,

	SNGraphicsResEnd = SNGraphicsResAppTitle,
	// -----------------------------------------------

	SNGraphicsResNum,
};

class SNGraphicsResource
{
public:
	static constexpr String ResourceInfoTable[SNGraphicsResNum] =
	{
		// -----------------------------------------------
		// 起動リソース
		(String)L"GraphicsResource.png",
		(String)L"起動ロゴ1.png",
		(String)L"起動ロゴ2.png",
		(String)L"湖とお城.png",
		// -----------------------------------------------
	};
};
