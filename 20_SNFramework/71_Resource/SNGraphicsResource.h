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

	SNGraphicsResExFont01 = SNGraphicsResSystemTop,
	SNGraphicsResExFont02,
	SNGraphicsResExFont03,
	SNGraphicsResExFont04,
	SNGraphicsResExFont05,
	SNGraphicsResExFont06,
	SNGraphicsResExFont07,
	SNGraphicsResExFont08,
	SNGraphicsResExFont09,
	SNGraphicsResExFont10,
	SNGraphicsResExFont11,
	SNGraphicsResStartLogo1,
	SNGraphicsResStartLogo2,
	SNGraphicsResWindow,
	SNGraphicsResButton,
	SNGraphicsResFocus,

	SNGraphicsResSystemEnd = SNGraphicsResFocus,
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
		(String)L"システムフォント.png", 
		// -----------------------------------------------
		
		// -----------------------------------------------
		// システムリソース
		(String)L"拡張フォント01.png",
		(String)L"拡張フォント02.png",
		(String)L"拡張フォント03.png",
		(String)L"拡張フォント04.png",
		(String)L"拡張フォント05.png",
		(String)L"拡張フォント06.png",
		(String)L"拡張フォント07.png",
		(String)L"拡張フォント08.png",
		(String)L"拡張フォント09.png",
		(String)L"拡張フォント10.png",
		(String)L"拡張フォント11.png",
		(String)L"起動ロゴ1.png",
		(String)L"起動ロゴ2.png",
		(String)L"ウインドウ.png",
		(String)L"ボタン.png",
		(String)L"フォーカス.png",
		// -----------------------------------------------

		// -----------------------------------------------
		// アプリケーションリソース
		(String)L"湖とお城.png",
		// -----------------------------------------------
	};
};
