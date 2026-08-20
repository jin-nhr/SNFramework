#pragma once
#include "SNFrameworkInternal.h"


class SNGUIDef
{
public:
	////////////////////////////////////////////////////////////////////////////////
	// 共通
	enum GUIFontColor
	{
		FontColorLight,
		FontColorDark,
		FontColorNum
	};

	enum GUIBlockIndex
	{
		GUIBlockLeftUp,    GUIBlockUp,     GUIBlockRightUp,
		GUIBlockLeft,      GUIBlockCenter, GUIBlockRight,
		GUIBlockLeftBottom,GUIBlockBottom, GUIBlockRightBottom,
		GUIBlockNum
	};

	static constexpr SNPoint GUIBlockDef[GUIBlockNum] =
	{
		{ 0,                                       0                                }, // 左上
		{ SNSystemConfig::GUIBlockSize,            0                                }, // 上
		{ SNSystemConfig::GUIBlockSize * 2,        0                                }, // 右上
		{ 0,                                       SNSystemConfig::GUIBlockSize     }, // 左
		{ SNSystemConfig::GUIBlockSize,            SNSystemConfig::GUIBlockSize     }, // 中央
		{ SNSystemConfig::GUIBlockSize * 2,        SNSystemConfig::GUIBlockSize     }, // 右
		{ 0,                                       SNSystemConfig::GUIBlockSize * 2 }, // 左下
		{ SNSystemConfig::GUIBlockSize,			   SNSystemConfig::GUIBlockSize * 2 }, // 下
		{ SNSystemConfig::GUIBlockSize * 2,        SNSystemConfig::GUIBlockSize * 2 }  // 右下
	};


	static constexpr SNColor FontColor[FontColorNum] =
	{
		{ 224, 208, 192, 255 },
		{  64,  32,  16, 255,}
	};

	static constexpr GUIFontColor FontColorNormal = FontColorDark;

	////////////////////////////////////////////////////////////////////////////////
	// フォーカス関連

	// ブロック構成はボタン準拠

	enum ButtonFocusStatus
	{
		FocusStatusNoFocus,
		FocusStatusNormal,
		FocusnStatusPush,
		FocusnStatusNum
	};

	static constexpr SNPoint FocusBlockOffset[FocusnStatusNum] =
	{
		{ 0,                                       0                                },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 3 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 6 }
	};


	////////////////////////////////////////////////////////////////////////////////
	// ウインドウ関連
	static constexpr SNPoint WindowBlockOffset[1] =
	{
		{ 0,                                       0                                }
	};


	////////////////////////////////////////////////////////////////////////////////
	// ボタン関連


	enum ButtonBlockStatus
	{
		ButtonStatusNoFrame,
		ButtonStatusDisable,
		ButtonStatusNormal,
		ButtonStatusPush,
		ButtonStatusSelected,
		ButtonStatusNum
	};


	static constexpr SNPoint ButtonBlockOffset[ButtonStatusNum] =
	{
		{ 0,                                       0                                },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 3 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 6 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 9 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 12}
	};

	static constexpr SNPoint ButtonCaptionOffset =
	{
		SNSystemConfig::GUIBlockSize / 2,
		(SNBitmapFont::BMCharHeight - SNSystemConfig::GUIBlockSize) / 2
	};

	static constexpr GUIFontColor ButtonCaptionColor[ButtonStatusNum] =
	{
		FontColorDark,	// Hidden
		FontColorLight,	// Disable
		FontColorLight,	// Normal
		FontColorLight,	// Push
		FontColorDark,	// Selected
	};

	////////////////////////////////////////////////////////////////////////////////
};
