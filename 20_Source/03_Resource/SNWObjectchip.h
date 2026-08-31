#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"

class SNWObjectchip
{
public:
	static constexpr SNGraphicsResID ObjectchipResource[] =
	{
		SNGraphicsResChara1,
	};


	// ブロック数
	static constexpr UInt32 WObjectBlockNumX = 8;
	static constexpr UInt32 WObjectBlockNumY = 1;
	static constexpr UInt32 WObjectBlockNum = WObjectBlockNumX * WObjectBlockNumY;

	// ブロックサイズ
	static constexpr UInt32 WObjectBlockSizeX = 64 * 3;
	static constexpr UInt32 WObjectBlockSizeY = 64 * 8;

	static constexpr UInt32 WObjectchipCodeMask = 0x00FF;
	static constexpr UInt32 WObjectchipResIDMask = 0xFF00;
	static constexpr UInt32 WObjectchipResIDSihit = 8;

	// アクション
	enum SNWActState
	{
		SNActStateIdle,
		SNActStateMove1,
		SNActStateMove2,
	};

	// ブロックオフセット
	static constexpr SNRect WObjectOffset[SNWorldDirNum] =
	{
		{ 0,  0, 64, 64 },	// Center
		{ 0,  0, 64, 64 },	// N
		{ 0, 64, 64, 64 },	// NE
		{ 0,128, 64, 64 },	// E
		{ 0,192, 64, 64 },	// SE
		{ 0,256, 64, 64 },	// S
		{ 0,320, 64, 64 },	// SW
		{ 0,384, 64, 64 },	// W
		{ 0,448, 64, 64 },	// NW
	};


	// 中心オフセット
	static constexpr SNPoint WObjectCenterOffset[SNWorldDirNum] =
	{
		{32, 60 },	// Center
		{32, 60 },	// N
		{32, 60 },	// NE
		{32, 60 },	// E
		{32, 60 },	// SE
		{32, 60 },	// S
		{32, 60 },	// SW
		{32, 60 },	// W
		{32, 60 },	// NW
	};

	static constexpr Int32 AnimationStepNum = 8;	// 最大コマ数

	// アニメーションステップ
	struct SNWAnimationStep
	{
		SNWActState	State;		// 状態
		Int32	Wait;		// 待ち時間(ms)
	};

	// アニメーション情報
	struct SNWAnimationInfo
	{
		Boolean Loop;
		SNWAnimationStep StepInfo[AnimationStepNum];
	};


	enum SNAnimationCode
	{
		SNAnimationCodeWait,
		SNAnimationCodeWalk,
		SNAnimationCodeNum
	};


	static constexpr SNWAnimationInfo AnimationInfo[SNAnimationCodeNum] =
	{
		// Wait
		{
			true,
			SNActStateIdle, 1,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
			SNActStateIdle,	0,
		},

		// Walk
		{
			true,
			SNActStateMove1, 300,
			SNActStateIdle,  300,
			SNActStateMove1, 300,
			SNActStateIdle,  300,
			SNActStateIdle,	 0,
			SNActStateIdle,	 0,
			SNActStateIdle,	 0,
			SNActStateIdle,	 0,
		}
	};


	struct WObjectChipData
	{
		UInt16 Code;										// オブジェクトコード
		SNAnimationCode AnimeCode[SNWObjectStateNum];		// アニメコード		
	};


	enum WObjectCode
	{
		WObjectCodeMaleOrg,
		WObjectCodeNum,
	};

	static constexpr WObjectChipData	Data[WObjectCodeNum] =
	{
		{
			0x0000,
			SNAnimationCodeWait,	// Idle
			SNAnimationCodeWalk,	// Walk
			SNAnimationCodeWalk,	// Jog
			SNAnimationCodeWait,	// Wait
			SNAnimationCodeWait,	// Jump
			SNAnimationCodeWait,	// Attack
			SNAnimationCodeWait,	// Knockback
			SNAnimationCodeWait,	// Spell
			SNAnimationCodeWait,	// OtherAction
			SNAnimationCodeWait,	// Flying
			SNAnimationCodeWait,	// Fall
		},

	};

	static inline Void CodeToRect(UInt16 code, Int32 dir, SNWActState state, SNRect* out_rect)
	{
		UInt16 tmp_code = (code & WObjectchipCodeMask);

		out_rect->PointX = (tmp_code / WObjectBlockNumY) * WObjectBlockSizeX + WObjectOffset[dir].PointX + WObjectOffset[dir].Width * state;
		out_rect->PointY = (tmp_code % WObjectBlockNumY) * WObjectBlockSizeY + WObjectOffset[dir].PointY;
		out_rect->Width  = WObjectOffset[dir].Width;
		out_rect->Height = WObjectOffset[dir].Height;
		
		return;
	};

	static inline UInt16 CodeToResID(UInt16 code)
	{
		return ((code & WObjectchipCodeMask) >> WObjectchipResIDSihit);
	}
};
