#pragma once
#include "SNFrameworkInternal.h"
#include "SNSystemConfig.h"
#include "SNGraphicsContext.h"
#include "SNBitmap.h"
#include "SNBitmapFont.h"

// GUIクラス
class SNGUI
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

	static constexpr SNColor FontColor[FontColorNum] =
	{
		{ 255, 255, 255, 255 },
		{   0,   0,   0,   0,}
	};

	static constexpr GUIFontColor FontColorNormal = FontColorDark;

	////////////////////////////////////////////////////////////////////////////////
	// ウインドウ関連

	enum WindowBlockIndex
	{
		WindowBlockLeftUp,     WindowBlockUp,     WindowBlockRightUp,
		WindowBlockLeft,       WindowBlockCenter, WindowBlockRight,
		WindowBlockLeftBottom, WindowBlockBottom, WindowBlockRightBottom,
		WindowBlockNum
	};

	static constexpr SNPoint WindowBlockDef[WindowBlockNum] =
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


	////////////////////////////////////////////////////////////////////////////////
	// ボタン関連

	enum ButtonBlockIndex
	{
		ButtonBlockLeftUp,     ButtonBlockUp,     ButtonBlockRightUp,
		ButtonBlockLeftBottom, ButtonBlockBottom, ButtonBlockRightBottom,
		ButtonBlockNum
	};

	enum ButtonBlockStatus
	{
		ButtonStatusHidden,
		ButtonStatusDisable,
		ButtonStatusNormal,
		ButtonStatusPush,
		ButtonStatusSelected,
		ButtonStatusNum
	};

	static constexpr SNPoint ButtonBlockDef[ButtonBlockNum] =
	{
		{ 0,                                       0                                }, // 左上
		{ SNSystemConfig::GUIBlockSize,            0                                }, // 上
		{ SNSystemConfig::GUIBlockSize * 2,        0                                }, // 右上
		{ 0,                                       SNSystemConfig::GUIBlockSize     }, // 左下
		{ SNSystemConfig::GUIBlockSize,            SNSystemConfig::GUIBlockSize     }, // 下
		{ SNSystemConfig::GUIBlockSize * 2,        SNSystemConfig::GUIBlockSize     }, // 下
	};

	static constexpr SNPoint ButtonBlockOffset[ButtonStatusNum] =
	{
		{ 0,                                       0                                },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 2 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 4 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 6 },
		{ 0,                                       SNSystemConfig::GUIBlockSize * 8 }
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





public:
	static Void Tiling(SNGraphicsContext* grc, SNRect* dst_rect, SNBitmap* src_bmp, SNRect* src_rect);



public:
	// 初期化処理
	static Void Initialize();

	// 起動準備
	static Void Startup();

	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();

	// 終了
	static Void Terminate();

};
