#pragma once
#include "SNFrameworkInternal.h"


// グラフィックリソースIDを定義する

enum SNGraphicsResID
{
	// -----------------------------------------------
	// 起動リソース
	SNGraphicsResStartupTop,

	SNGraphicsResGUI = SNGraphicsResStartupTop,
	SNGraphicsResSystemBlack,

	SNGraphicsResStartupEnd = SNGraphicsResSystemBlack,
	// -----------------------------------------------

	// -----------------------------------------------
	// システムリソース
	SNGraphicsResSystemTop,
	SNGraphicsResStartLogo1 = SNGraphicsResSystemTop,
	SNGraphicsResStartLogo2,
	SNGraphicsResWindow,
	SNGraphicsResButton,
	SNGraphicsResFocus,

	SNGraphicsResSystemEnd = SNGraphicsResFocus,
	// -----------------------------------------------

	// -----------------------------------------------
	// アプリケーションリソース
	SNGraphicsResAppTop,

	SNGraphicsResAppTitle = SNGraphicsResAppTop,
	SNGraphicsResBG1,
	SNGraphicsResBG2,
	SNGraphicsResBG3,
	SNGraphicsResBG4,
	SNGraphicsResMapchip1,
	SNGraphicsResChara1,

	SNGraphicsResAppEnd = SNGraphicsResChara1,
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
		(String)L"GUI.png",
		(String)L"BGBK.png",
		// -----------------------------------------------
		
		// -----------------------------------------------
		// システムリソース
		(String)L"起動ロゴ1.png",
		(String)L"起動ロゴ2.png",
		(String)L"ウインドウ.png",
		(String)L"ボタン.png",
		(String)L"フォーカス.png",
		// -----------------------------------------------

		// -----------------------------------------------
		// アプリケーションリソース
		(String)L"湖とお城.png",
		(String)L"BG1.png",
		(String)L"BG2.png",
		(String)L"BG3.png",
		(String)L"BG4.png",
		(String)L"マップチップ1.png",
		(String)L"キャラクター1.png",
		// -----------------------------------------------
	};
};
