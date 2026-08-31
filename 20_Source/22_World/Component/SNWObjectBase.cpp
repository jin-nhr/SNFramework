#include "SNWObjectBase.h"
#include "SNMath.h"

SNWObjectBase::SNWObjectBase()
{
	InitObjectInfo();

	return;
}

SNWObjectBase::~SNWObjectBase()
{
	return;
}

Void SNWObjectBase::Initialize()
{
	InitObjectInfo();

	return;
}

Void SNWObjectBase::Terminate()
{
	return;
}

// ObjectInfo参照
SNWObjectInfo* SNWObjectBase::RefInfo()
{
	return &Info;
}

Void SNWObjectBase::InitObjectInfo()
{
	SNWObjectInfo* info;

	info = &Info;

	info->Code = SNWObjectchip::WObjectCodeMaleOrg;
	info->AnimationStep = 0;
	info->AnimationCount = 0;
	info->Enable = false;
	info->Visible = false;
	info->Weight = false;
	info->State = SNWObjectStateIdle;
	info->FrontDir = SNWorldDirS;
	info->Pos.X = 0;
	info->Pos.Y = 0;
	info->Pos.Z = 0;
	info->Accelaration.X = 0;
	info->Accelaration.Y = 0;
	info->Accelaration.Z = 0;
	info->Speed.X = 0;
	info->Speed.Y = 0;
	info->Speed.Z = 0;

	return;
}

// コード取得
UInt16 SNWObjectBase::GetCode()
{
	return SNWObjectchip::Data[Info.Code].Code;
}

// アニメステート取得
SNWObjectchip::SNWActState SNWObjectBase::GetActState()
{
	SNWObjectchip::SNAnimationCode anm_code = SNWObjectchip::Data[Info.Code].AnimeCode[Info.State];
	SNWObjectchip::SNWActState act_code = SNWObjectchip::AnimationInfo[anm_code].StepInfo[Info.AnimationStep].State;

	return act_code;
}

// 方向計算
SNWorldDir SNWObjectBase::CalcDir(SNWorldDir base_dir)
{
	return (SNWorldDir)(((Info.FrontDir - SNWorldDirN + SNWorldDirNW) + (base_dir - SNWorldDirN)) % (SNWorldDirNW) + SNWorldDirN);

}
