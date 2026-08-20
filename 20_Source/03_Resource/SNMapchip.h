#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"

class SNMapchip
{
public:
	// マップチップリソース
	static constexpr SNGraphicsResID MapchipResource = SNGraphicsResMapchip1;

	// ブロック数
	static constexpr UInt32 MapchipBlockNumX = 16;
	static constexpr UInt32 MapchipBlockNumY = 16;
	static constexpr UInt32 MapchipBlockNum = MapchipBlockNumX * MapchipBlockNumY;

	// ブロックサイズ
	static constexpr UInt32 MapchipBlockSizeX = 16;
	static constexpr UInt32 MapchipBlockSizeY = 32;

	// ブロックオフセット
	static constexpr SNRect MapchipOffset[SNWorldDirNum] =
	{
		{ 2,17, 12, 14 },	// Center
		{ 2,17, 12, 14 },	// N
		{ 0, 0, 16, 16 },	// NE
		{ 2,17, 12, 14 },	// E
		{ 0, 0, 16, 16 },	// SE
		{ 2,17, 12, 14 },	// S
		{ 0, 0, 16, 16 },	// SW
		{ 2,17, 12, 14 },	// W
		{ 0, 0, 16, 16 },	// NW
	};


	// 中心オフセット
	static constexpr SNPoint MapchipCenterOffset[SNWorldDirNum] =
	{
		{ 6,  3 },	// Center
		{ 6,  3 },	// N
		{ 8,  4 },	// NE
		{ 6,  3 },	// E
		{ 8,  4 },	// SE
		{ 6,  3 },	// S
		{ 8,  4 },	// SW
		{ 6,  3 },	// W
		{ 8,  4 },	// NW
	};



	// ストライドX
	static constexpr SNPoint MapchipStrideX[SNWorldDirNum] =
	{
		{12,  0 },	// Center
		{12,  0 },	// N
		{ 8,  4 },	// NE
		{ 0, 12 },	// E
		{-8,  4 },	// SE
		{12,  0 },	// S
		{ 8,  4 },	// SW
		{ 0, 12 },	// W
		{-8,  4 },	// NW
	};

	// ストライドY
	static constexpr SNPoint MapchipStrideY[SNWorldDirNum] =
	{
		{ 0, 6 },	// Center
		{ 0, 6 },	// N
		{-8, 4 },	// NE
		{ 0, 6 },	// E
		{ 8, 4 },	// SE
		{ 0, 6 },	// S
		{-8, 4 },	// SW
		{ 0, 6 },	// W
		{ 8, 4 },	// NW
	};

	// ストライドZ
	static constexpr SNPoint MapchipStrideZ[SNWorldDirNum] =
	{
		{ 0, -8 },	// Center
		{ 0, -8 },	// N
		{ 0, -8 },	// NE
		{ 0, -8 },	// E
		{ 0, -8 },	// SE
		{ 0, -8 },	// S
		{ 0, -8 },	// SW
		{ 0, -8 },	// W
		{ 0, -8 },	// NW
	};



	// GUIブロック
	static constexpr UInt16 FocusCode = 0x0001;		// フォーカス
	static constexpr UInt16 GridCode = 0x0002;		// グリッド

	// 影ブロック
	static constexpr UInt16 ShadowCode[SNWorldShadowDirNum] =
	{
		0x0000,
		MapchipBlockNumY + 0x0001,	// 左上
		MapchipBlockNumY + 0x0002,	// 左下
		MapchipBlockNumY + 0x0003,	// 右下
		MapchipBlockNumY + 0x0004,	// 右上
		MapchipBlockNumY + 0x0005,	// 上
	};

	// 境界　ブロック
	static constexpr UInt16 BorderCode[SNWorldGroundBorderDirNum] =
	{
		0x0000,
		MapchipBlockNumY + 0x0006,	// 右
		MapchipBlockNumY + 0x0007,	// 左
		MapchipBlockNumY + 0x0008,	// 上
	};


	// マップチップデータ
	struct SNMapchipData
	{
		UInt16			Code;
		Boolean			Collision;
	};

	// マップチップコード
	enum SNMapchipCode
	{
		SNMapchipBlank,
		SNMapchipGreen,
		SNMapchipLightGreen,
		SNMapchipDeepGreen,
		SNMapchipSakura,
		SNMapchipIchou,
		SNMapchipKaede,
		SNMapchipDeadLeaf,
		SNMapchipSea,
		SNMapchipDeepSea,
		SNMapchipMagma,
		SNMapchipPoisonSwamp,
		SNMapchipDirt,
		SNMapchipRedDirt,
		SNMapchipBlackDirt,
		SNMapchipSand,
		SNMapchipSnow,
		SNMapchipStone,
		SNMapchipRedBrick1,
		SNMapchipRedBrick2,
		SNMapchipWhiteBrick1,
		SNMapchipWhiteBrick2,
		SNMapchipNum
	};

	// マップチップデータ
	static constexpr SNMapchipData Data[SNMapchipNum] =
	{
		{	0x0000,			false		},		// Blank
		{	0x0020,			true			},		// Green
		{	0x0021,			true			},		// LightGreen
		{	0x0022,			true			},		// DeepGreen
		{	0x0023,			true			},		// Sakura
		{	0x0024,			true			},		// Ishou
		{	0x0025,			true			},		// Kaede
		{	0x0026,			true			},		// DeadLeaf
		{	0x0030,			true			},		// Sea
		{	0x0031,			true			},		// DeepSea
		{	0x0032,			true			},		// Magma
		{	0x0033,			true			},		// PoisonSwamp
		{	0x0040,			true			},		// Dirt
		{	0x0041,			true			},		// RedDirt
		{	0x0042,			true			},		// BlackDirt
		{	0x0043,			true			},		// Sand
		{	0x0044,			true			},		// Snow
		{	0x0045,			true			},		// Stone
		{	0x0050,			true			},		// RedBrick1
		{	0x0051,			true			},		// RedBrick2
		{	0x0052,			true			},		// WhiteBrick1
		{	0x0053,			true			},		// WhiteBrick2
	};


	static inline Void CodeToRect(UInt16 code, Int32 dir, SNRect* out_rect)
	{
		out_rect->PointX = (code / MapchipBlockNumY) * MapchipBlockSizeX + MapchipOffset[dir].PointX;
		out_rect->PointY = (code % MapchipBlockNumY) * MapchipBlockSizeY + MapchipOffset[dir].PointY;
		out_rect->Width  = MapchipOffset[dir].Width;
		out_rect->Height = MapchipOffset[dir].Height;
		
		return;
	};
};

