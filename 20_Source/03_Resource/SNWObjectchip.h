#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"

class SNWObjectchip
{
public:

	static constexpr SNGraphicsResID ObjectchipResource = SNGraphicsResGUIAndWorld;

	static constexpr UInt32 WObjectBaseX = 512;
	static constexpr UInt32 WObjectBaseY = 0;

	static constexpr Int32 AnimationStepNum = 8;	// 最大コマ数

	// アニメーションステップ
	struct SNWAnimationStep
	{
		UInt8	State;		// 表示列
		Int32	Wait;		// 待ち時間(ms)
	};

	// アニメーション情報
	struct SNWAnimationInfo
	{
		Boolean Loop;
		UInt8 StepNum;
		SNWAnimationStep StepInfo[AnimationStepNum];
	};


	struct WObjectChipData
	{
		SNPoint ImgOffset;
		SNSize  ImgSize;
		SNPoint imgCenterOffset;
		UInt16 imgDirOffset[SNWorldDirNum];
		Float32 SizeH;
		Float32 SizeV;
		Boolean Gravity;
		Boolean Flyable;
		SNWAnimationInfo AnimeCode[SNWObjectStateNum];		// アニメコード	
		Float32 SpeedWalk;
		Float32 SpeedJump;
		Float32 SpeedFlying;
		Float32 SpeedSwimming;
	};


	enum WObjectCode
	{
		WObjectCodeBone,
		WObjectCodeSlime,
		WObjectCodeBat,
		WObjectCodeNum,
	};

	static constexpr WObjectChipData	Data[WObjectCodeNum] =
	{
		// Bone
		{
			// Img offset
			0, 0,

			// img size
			32, 32,

			// center offset
			16, 27,

			// dir offset (c, n, ne, e, se, s, sw, w, nw)
			0, 0, 0, 1, 1, 2, 2, 2, 3,

			// Size
			1, 3,

			// Ability
			// gravity, fly
			true,       false,

			// Animation
			// Idle
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Walk
			true, 8,
			2, 100,
			3, 100,
			4, 100,
			5, 100,
			6, 100,
			7, 100,
			8, 100,
			1, 100,

			// Jump
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Attack
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Knockback
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			
			// Spell
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// OtherAction
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Flying
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Fall
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Swimming
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Speed (blk/sec) ※ 1フレームでの移動距離は1blk上限とする (60fpsなら16.6くらい)
			3.0f,			// Walk
			10.0f,			// Jump
			1.0f,			// Flying
			1.0f			// Swimming
		},

		// Slime
		{
			// Img offset
			0, 512,

			// img size
			32, 32,

			// center offset
			16, 27,

			// dir offset (c, n, ne, e, se, s, sw, w, nw)
			0, 0, 0, 1, 1, 2, 2, 2, 3,

			// Size
			1, 1,

			// Ability
			// gravity, fly
			true,       false,

			// Animation
			// Idle
			true, 2,
			0, 300,
			1, 300,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Walk
			true, 5,
			1, 100,
			2, 100,
			0, 100,
			3, 100,
			0, 100,
			0, 0,
			0, 0,
			0, 0,

			// Jump
			false, 1,
			1, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Attack
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Knockback
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Spell
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// OtherAction
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Flying
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Fall
			false, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Swimming
			true, 1,
			0, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Speed (blk/sec) ※ 1フレームでの移動距離は1blk上限とする (60fpsなら16.6くらい)
			2.0f,			// Walk
			10.0f,			// Jump
			1.0f,			// Flying
			1.0f			// Swimming
		},

		// Bat
		{
			// Img offset
			0, 640,

			// img size
			32, 32,

			// center offset
			16, 27,

			// dir offset (c, n, ne, e, se, s, sw, w, nw)
			0, 0, 0, 1, 1, 2, 2, 2, 3,

			// Size
			1, 1,

			// Ability
			// gravity, fly
			true,       true,

			// Animation
			// Idle
			true, 1,
			1, 1,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Walk
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Jump
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Attack
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Knockback
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Spell
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// OtherAction
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Flying
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Fall
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Swimming
			true, 2,
			0, 100,
			1, 100,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,

			// Speed (blk/sec) ※ 1フレームでの移動距離は1blk上限とする (60fpsなら16.6くらい)
			3.0f,			// Walk
			6.0f,			// Jump
			4.0f,			// Flying
			1.0f			// Swimming
		},

	};

	static inline Void CodeToRect(UInt16 code, Int32 dir, UInt8 anm_state, UInt8 anm_step, SNRect* out_rect)
	{
		out_rect->PointX = WObjectBaseX + Data[code].ImgOffset.X + Data[code].ImgSize.Width * (Data[code].AnimeCode[anm_state].StepInfo[anm_step].State);
		out_rect->PointY = WObjectBaseY + Data[code].ImgOffset.Y + Data[code].ImgSize.Height * Data[code].imgDirOffset[dir];
		out_rect->Width = Data[code].ImgSize.Width;
		out_rect->Height = Data[code].ImgSize.Height;

		return;
	};

};
