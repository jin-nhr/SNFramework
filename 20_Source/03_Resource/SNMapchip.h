#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"

class SNMapchip
{
public:
	// マップチップリソース
	static constexpr SNGraphicsResID MapchipResource = SNGraphicsResWorld;

	static constexpr UInt32 MapchipBaseX = 0;
	static constexpr UInt32 MapchipBaseY = 0;

	static constexpr UInt32 MapchipOffsetX = 0;
	static constexpr UInt32 MapchipOffsetY = 384;

	static constexpr Int32 MapchipAnimeStep = 4;

	// ブロック数
	static constexpr UInt32 MapchipBlockNumX = 16;
	static constexpr UInt32 MapchipBlockNumY = 16;
	static constexpr UInt32 MapchipBlockNum = MapchipBlockNumX * MapchipBlockNumY;

	// ブロックサイズ
	static constexpr UInt32 MapchipBlockSizeX = 16;
	static constexpr UInt32 MapchipBlockSizeY = 32;

	static constexpr UInt32 SNMapchipCodeMask = 0x00FF;
	static constexpr UInt32 SNMapchipResIDMask = 0xFF00;
	static constexpr UInt32 SNMapchipResIDSihit = 8;

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

	// 底面中心オフセット
	static constexpr SNPoint MapchipBottomCenterOffset[SNWorldDirNum] =
	{
		{ 6,  11 },	// Center
		{ 6,  11 },	// N
		{ 8,  12 },	// NE
		{ 6,  11 },	// E
		{ 8,  12 },	// SE
		{ 6,  11 },	// S
		{ 8,  12 },	// SW
		{ 6,  11 },	// W
		{ 8,  12 },	// NW
	};

	// ストライドX
	static constexpr SNPoint MapchipStrideX[SNWorldDirNum] =
	{
		{12,  0 },	// Center
		{12,  0 },	// N
		{ 8,  4 },	// NE
		{ 0,  6 },	// E
		{-8,  4 },	// SE
		{-12, 0 },	// S
		{-8, -4 },	// SW
		{ 0, -6 },	// W
		{ 8, -4 },	// NW
	};

	// ストライドY
	static constexpr SNPoint MapchipStrideY[SNWorldDirNum] =
	{
		{ 0, 6 },	// Center
		{ 0, 6 },	// N
		{-8, 4 },	// NE
		{-12,0 },	// E
		{-8,-4 },	// SE
		{ 0,-6 },	// S
		{ 8,-4 },	// SW
		{12, 0 },	// W
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
	static constexpr UInt16 SelectedCode = 0x0002;	// 選択
	static constexpr UInt16 GridCode = 0x0003;		// グリッド
	
	
	// 影ブロック
	static constexpr UInt16 ShadowCode[SNWorldShadowDirNum] =
	{
		0x0000,
		MapchipBlockNumY + 0x0000,
		MapchipBlockNumY + 0x0001,
		MapchipBlockNumY + 0x0002,
		MapchipBlockNumY + 0x0003,
		MapchipBlockNumY + 0x0004,
		MapchipBlockNumY + 0x0005,
		MapchipBlockNumY + 0x0006,

	};

	// 境界　ブロック
	static constexpr UInt16 BorderCode[SNWorldGroundBorderDirNum] =
	{
		0x0000,
		MapchipBlockNumY + 0x0007,
		MapchipBlockNumY + 0x0008,
		MapchipBlockNumY + 0x0009,
		MapchipBlockNumY + 0x000A,
		MapchipBlockNumY + 0x000B,
		MapchipBlockNumY + 0x000C,
		MapchipBlockNumY + 0x000D,
		MapchipBlockNumY + 0x000E,
	};


	// マップチップデータ
	struct SNMapchipData
	{
		UInt16			Code[MapchipAnimeStep];
		Boolean			Visible;
		Boolean			Collision;
		Boolean			Light;
		Boolean			Shadow;
		Float32			MoveScale;
		Float32			JumpScale;
		Float32			FallScale;
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
		//	[0]		[1]		[2]		[3]		Visible	Collision	Light	Shadow	MoveScale	JumpScale	FallScale	
		{	0x0000,	0x0000,	0x0000,	0x0000,	false,	false,		false,	false,	0.10f,		0.0f,		1.0f,		},		// Blank
		{	0x0020,	0x0020,	0x0020,	0x0020,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Green
		{	0x0021,	0x0021,	0x0021,	0x0021,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// LightGreen
		{	0x0022,	0x0022,	0x0022,	0x0022,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// DeepGreen
		{	0x0023,	0x0023,	0x0023,	0x0023,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Sakura
		{	0x0024,	0x0024,	0x0024,	0x0024,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Ishou
		{	0x0025,	0x0025,	0x0025,	0x0025,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Kaede
		{	0x0026,	0x0026,	0x0026,	0x0026,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// DeadLeaf
		{	0x0030,	0x0031,	0x0032,	0x0033,	true,	false,		false,	true,	0.20f,		0.4f,		0.2f,		},		// Sea
		{	0x0034,	0x0035,	0x0036,	0x0037,	true,	false,		false,	true,	0.20f,		0.4f,		0.2f,		},		// DeepSea
		{	0x0038,	0x0039,	0x003A,	0x003B,	true,	false,		false,	true,	0.20f,		0.4f,		0.2f,		},		// Magma
		{	0x003C,	0x003D,	0x003E,	0x003F,	true,	false,		false,	true,	0.20f,		0.4f,		0.2f,		},		// PoisonSwamp
		{	0x0040,	0x0040,	0x0040,	0x0040,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Dirt
		{	0x0041,	0x0041,	0x0041,	0x0041,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// RedDirt
		{	0x0042,	0x0042,	0x0042,	0x0042,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// BlackDirt
		{	0x0043,	0x0043,	0x0043,	0x0043,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Sand
		{	0x0044,	0x0044,	0x0044,	0x0044,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Snow
		{	0x0045,	0x0045,	0x0045,	0x0045,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// Stone
		{	0x0050,	0x0050,	0x0050,	0x0050,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// RedBrick1
		{	0x0051,	0x0051,	0x0051,	0x0051,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// RedBrick2
		{	0x0052,	0x0052,	0x0052,	0x0052,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// WhiteBrick1
		{	0x0053,	0x0053,	0x0053,	0x0053,	true,	true,		false,	true,	0.30f,		1.0f,		0.0f,		},		// WhiteBrick2
	};
		
			
	static inline Void CodeToRect(UInt16 code, Int32 dir, SNRect* out_rect)
	{
		UInt16 res_id = CodeToResID(code);
		UInt16 tmp_code = (code & SNMapchipCodeMask);

		out_rect->PointX = MapchipBaseX + (tmp_code / MapchipBlockNumY) * MapchipBlockSizeX + MapchipOffset[dir].PointX + MapchipOffsetX * res_id;
		out_rect->PointY = MapchipBaseX + (tmp_code % MapchipBlockNumY) * MapchipBlockSizeY + MapchipOffset[dir].PointY + MapchipOffsetY * res_id;
		out_rect->Width  = MapchipOffset[dir].Width;
		out_rect->Height = MapchipOffset[dir].Height;
		
		return;
	};

	static inline UInt16 CodeToResID(UInt16	code)
	{
		return ((code & SNMapchipCodeMask) >> SNMapchipResIDSihit);
	}
};

