#pragma once
#include "SNFrameworkInternal.h"


// グラフィックリソースIDを定義する

enum SNGraphicsResID
{
	// -----------------------------------------------
	// 起動リソース
	SNGraphicsResStartupTop,

	SNGraphicsResSystemFont = SNGraphicsResStartupTop,

	SNGraphicsResStartupEnd = SNGraphicsResSystemFont,
	// -----------------------------------------------

	// -----------------------------------------------
	// システムリソース
	SNGraphicsResSystemTop,

	SNGraphicsResStartLogo1 = SNGraphicsResSystemTop,
	SNGraphicsResStartLogo2,

	SNGraphicsResSystemEnd = SNGraphicsResStartLogo2,
	// -----------------------------------------------

	// -----------------------------------------------
	// アプリケーションリソース
	SNGraphicsResAppTop,

	SNGraphicsResAppDummy = SNGraphicsResAppTop,

	SNGraphicsResAppEnd = SNGraphicsResAppDummy,
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
		(String)L"ビットマップフォント.png", 
		// -----------------------------------------------
		
		// -----------------------------------------------
		// システムリソース
		(String)L"起動ロゴ1.png",
		(String)L"起動ロゴ2.png",
		// -----------------------------------------------

		// -----------------------------------------------
		// アプリケーションリソース
		(String)L"湖とお城.png",
		// -----------------------------------------------
	};
};
