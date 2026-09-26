#pragma once
#include "SNFrameworkInternal.h"
#include "SNSystemConfig.h"

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

	static constexpr SNColor FontColor[FontColorNum] =
	{
		{ 224, 208, 192, 255 },
		{  64,  32,  16, 255,}
	};

	static constexpr GUIFontColor FontColorSystem = FontColorLight;
	static constexpr GUIFontColor FontColorNormal = FontColorDark;


	////////////////////////////////////////////////////////////////////////////////
	// ビットマップフォント定義

	// ビットマップフォントテーブル数
	static constexpr UInt32 BMCharTableNum = 256;

	// ビットマップフォント幅/高さ
	static constexpr UInt32 BMCharWidth = 16;
	static constexpr UInt32 BMCharHeight = 24;

	// ビットマップフォント縦横ブロック数
	static constexpr UInt32 BMCharBlockNumX = 16;
	static constexpr UInt32 BMCharBlockNumY = 16;

	// ビットマップフォントベース座標
	static constexpr UInt32 BMCharBaseX = 0;
	static constexpr UInt32 BMCharBaseY = 1664;

	// ページオフセット
	static constexpr UInt32 BMCharPageOffsetX = 256;
	static constexpr UInt32 BMCharPageOffsetY = 0;


	////////////////////////////////////////////////////////////////////////////////
	// 背景定義
	
	// ベース座標
	static constexpr UInt32 GUIBGBaseX = 1;
	static constexpr UInt32 GUIBGBaseY = 1385;

	// 背景パターン
	enum GUIBGPattern
	{
		GUIBGPatternBlack,
	};

	// パターン毎のオフセット
	static constexpr UInt32 GUIBGOffsetX = 4;
	static constexpr UInt32 GUIBGOffsetY = 0;

	// サイズ
	static constexpr UInt32 GUIBGWidth = 2;
	static constexpr UInt32 GUIBGHeight = 254;




	////////////////////////////////////////////////////////////////////////////////
	// フォーカス関連

	// ブロック構成はボタン準拠

	static constexpr UInt32 GUIFocusBaseX = 0;
	static constexpr UInt32 GUIFocusBaseY = 48;

	enum ButtonFocusStatus
	{
		FocusStatusNoFocus,
		FocusStatusNormal,
		FocusnStatusPush,
		FocusnStatusNum
	};

	static constexpr SNPoint GUIFocusBlockDef[GUIBlockNum] =
	{
		{ GUIFocusBaseX,                                       GUIFocusBaseY                                }, // 左上
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize,        GUIFocusBaseY                                }, // 上
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIFocusBaseY                                }, // 右上
		{ GUIFocusBaseX,                                       GUIFocusBaseY + SNSystemConfig::GUIBlockSize     }, // 左
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize,        GUIFocusBaseY + SNSystemConfig::GUIBlockSize     }, // 中央
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIFocusBaseY + SNSystemConfig::GUIBlockSize     }, // 右
		{ GUIFocusBaseX,                                       GUIFocusBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 左下
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize,		   GUIFocusBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 下
		{ GUIFocusBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIFocusBaseY + SNSystemConfig::GUIBlockSize * 2 }  // 右下
	};


	static constexpr SNPoint FocusBlockOffset[FocusnStatusNum] =
	{
		{ 0,                                       0                                },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 3 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 6 }
	};


	////////////////////////////////////////////////////////////////////////////////
	// ウインドウ関連

	static constexpr UInt32 GUIWindowBaseX = 0;
	static constexpr UInt32 GUIWindowBaseY = 0;


	static constexpr SNPoint WindowBlockOffset[1] =
	{
		{ 0,                                       0                                }
	};

	static constexpr SNPoint GUIWindowBlockDef[GUIBlockNum] =
	{
		{ GUIWindowBaseX,                                       GUIWindowBaseY                                }, // 左上
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize,        GUIWindowBaseY                                }, // 上
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIWindowBaseY                                }, // 右上
		{ GUIWindowBaseX,                                       GUIWindowBaseY + SNSystemConfig::GUIBlockSize     }, // 左
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize,        GUIWindowBaseY + SNSystemConfig::GUIBlockSize     }, // 中央
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIWindowBaseY + SNSystemConfig::GUIBlockSize     }, // 右
		{ GUIWindowBaseX,                                       GUIWindowBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 左下
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize,		GUIWindowBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 下
		{ GUIWindowBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIWindowBaseY + SNSystemConfig::GUIBlockSize * 2 }  // 右下
	};


	////////////////////////////////////////////////////////////////////////////////
	// ボタン関連

	static constexpr UInt32 GUIButtonBaseX = 48;
	static constexpr UInt32 GUIButtonBaseY = 0;

	enum ButtonBlockStatus
	{
		ButtonStatusNoFrame,
		ButtonStatusDisable,
		ButtonStatusNormal,
		ButtonStatusPush,
		ButtonStatusSelected,
		ButtonStatusNum
	};


	static constexpr SNPoint GUIButtonBlockDef[GUIBlockNum] =
	{
		{ GUIButtonBaseX,                                       GUIButtonBaseY                                }, // 左上
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize,        GUIButtonBaseY                                }, // 上
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIButtonBaseY                                }, // 右上
		{ GUIButtonBaseX,                                       GUIButtonBaseY + SNSystemConfig::GUIBlockSize     }, // 左
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize,        GUIButtonBaseY + SNSystemConfig::GUIBlockSize     }, // 中央
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIButtonBaseY + SNSystemConfig::GUIBlockSize     }, // 右
		{ GUIButtonBaseX,                                       GUIButtonBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 左下
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize,		GUIButtonBaseY + SNSystemConfig::GUIBlockSize * 2 }, // 下
		{ GUIButtonBaseX + SNSystemConfig::GUIBlockSize * 2,    GUIButtonBaseY + SNSystemConfig::GUIBlockSize * 2 }  // 右下
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
		(BMCharHeight - SNSystemConfig::GUIBlockSize) / 2
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
