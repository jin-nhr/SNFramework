#include "SNWorld.h"
#include "SNSystemConfig.h"
#include "SNWGlobalObject.h"

SNWorldPos SNWorld::CurrentPos = {0};
SNWMeshManager SNWorld::MeshManager;
SNWGlobalObject SNWorld::GlobalObject;
UInt32 SNWorld::WorldTime = 0;
Boolean SNWorld::Run = false;
Boolean SNWorld::Suspend = false;
SNWNearbySpace SNWorld::NearbySpace;	// 周辺空間

SNWorldDir SNWorld::GlobalLight;
SNWEasyLightDir SNWorld::EasyLight;

// 初期化
Void SNWorld::Initialize()
{
	CurrentPos = {0};
	WorldTime = 0;
	Run = false;
	Suspend = false;

	GlobalLight = SNWorldDirSE;
	EasyLight = RefEasyLightDir();

	MeshManager.Initialize();
	NearbySpace.Initialize();

	GlobalObject.Initialize();
	GlobalObject.Load();

	return;
}

// 終了
Void SNWorld::Terminate()
{
	NearbySpace.Terminate();
	MeshManager.Terminate();

	GlobalObject.Save();
	GlobalObject.Terminate();

	return;
}

Void SNWorld::Start()
{
	Run = true;
	WorldTime = 0;
	return;
}

Void SNWorld::End()
{
	Run = false;
	return;
}

Void SNWorld::Pause()
{
	// 状態反転
	if (Suspend)
	{
		Suspend = false;
	}

	else
	{
		Suspend = true;
	}

	return;
}

// 更新
Void SNWorld::Update()
{
	if (Run && !Suspend)
	{
		// 更新準備
		WorldTime++;
		NearbySpace.UpdateStart(&CurrentPos, WorldTime);

		// 地形更新
		UpdateGround();

		// グローバルオブジェクト更新
		UpdateGlobalObject();


		// エフェクト登録
		RegisterNearbyEffect();
	}

	return;
}

// 地形更新
Void SNWorld::UpdateGround()
{
	// 地形更新
	MeshManager.Update(&CurrentPos);

	// 地形オブジェクト登録
	MeshManager.RegisterNearbyObject(&NearbySpace);

	return;
}

// グローバルオブジェクト更新
Void SNWorld::UpdateGlobalObject()
{
	// グローバルオブジェクト更新
	GlobalObject.Update();

	// グローバルオブジェクト登録
	GlobalObject.RegisterNearbyObject(&NearbySpace);

	return;
}


// カレント座標設定
Void SNWorld::SetCurrentPos(SNWorldPos* pos)
{
	CurrentPos = *pos;

	return;
}

// カレント座標移動
Void SNWorld::MoveCurrentPos(SNWorldPos* pos)
{
	CurrentPos.X += pos->X;
	CurrentPos.Y += pos->Y;
	CurrentPos.Z += pos->Z;

	return;
}

// 地形書き込み
Void SNWorld::WriteGroundData(SNMapchip::SNMapchipCode code)
{
	MeshManager.Write(code);

	return;
}

SNWNearbySpace* SNWorld::GetNearbySpace()
{
	return &NearbySpace;
}


SNWEasyLightDir SNWorld::RefEasyLightDir()
{
	SNWEasyLightDir ret = SNWEasyLightDirRight;


	switch (GlobalLight)
	{
	case SNWorldDirN:
	case SNWorldDirNE:
		ret = SNWEasyLightDirUp;
		break;
	case SNWorldDirE:
	case SNWorldDirSE:
		ret = SNWEasyLightDirRight;
		break;
	case SNWorldDirS:
	case SNWorldDirSW:
		ret = SNWEasyLightDirBottom;
		break;
	case SNWorldDirW:
	case SNWorldDirNW:
		ret = SNWEasyLightDirLeft;
		break;
	}

	return ret;
}


// 周辺空間へのエフェクト登録
Void SNWorld::RegisterNearbyEffect()
{
	SNWNearbyObject* obj_ptr = nullptr;
	UInt32 obj_num = NearbySpace.GetObjectNum();
	Int64 cnt;

	// 登録されたオブジェクトを走査

	for (cnt = 0; cnt < obj_num; cnt++)
	{
		obj_ptr = NearbySpace.RefObject((Int32)cnt);

		switch (obj_ptr->Type)
		{
		case SNWNearbyObjectTypeGround:
			RegisterNearbyEffectGround(obj_ptr);
			break;
		}
	}

	return;
}

Void SNWorld::RegisterNearbyEffectGround(SNWNearbyObject* obj_ptr)
{
	Int32 x, y, z;
	UInt64 effect_flg = 0;
	Boolean exist_u;
	Boolean exist_r;
	Boolean exist_b;
	Boolean exist_l;
	Boolean exist_top;

	Boolean exist_uu;
	Boolean exist_ru;
	Boolean exist_bu;
	Boolean exist_lu;

	Boolean exist_bottom;


	static UInt64 gshadow[SNWEasyLightDirNum] =
	{
		SNWNearbyEffectGroundBitGShadowU,
		SNWNearbyEffectGroundBitGShadowR,
		SNWNearbyEffectGroundBitGShadowB,
		SNWNearbyEffectGroundBitGShadowL
	};

	x = (Int32)obj_ptr->Pos.X;
	y = (Int32)obj_ptr->Pos.Y;
	z = (Int32)obj_ptr->Pos.Z;

	// 周囲4つが地形？
	exist_u = NearbySpace.IsBlocked(x, y - 1, z, SNWNearbyObjectTypeGround);
	exist_r = NearbySpace.IsBlocked(x + 1, y, z, SNWNearbyObjectTypeGround);
	exist_b = NearbySpace.IsBlocked(x, y + 1, z, SNWNearbyObjectTypeGround);
	exist_l = NearbySpace.IsBlocked(x - 1, y, z, SNWNearbyObjectTypeGround);

	exist_top = NearbySpace.IsBlocked(x, y, z + 1, SNWNearbyObjectTypeGround);

	exist_uu = NearbySpace.IsBlocked(x, y - 1, z + 1, SNWNearbyObjectTypeGround);
	exist_ru = NearbySpace.IsBlocked(x + 1, y, z + 1, SNWNearbyObjectTypeGround);
	exist_bu = NearbySpace.IsBlocked(x, y + 1, z + 1, SNWNearbyObjectTypeGround);
	exist_lu = NearbySpace.IsBlocked(x - 1, y, z + 1, SNWNearbyObjectTypeGround);

	exist_bottom = NearbySpace.IsBlocked(x, y, z - 1, SNWNearbyObjectTypeGround);

	// 周囲に地形以外のセルがある
	if (!(exist_u && exist_r && exist_b && exist_l))
	{
		// グローバル光源の反映
		effect_flg |= gshadow[EasyLight];

		if (!exist_top)
		{
			// ボーダーの設定 周りがないとき
			effect_flg |= (exist_u ? 0 : SNWNearbyEffectGroundBitBorderU);
			effect_flg |= (exist_r ? 0 : SNWNearbyEffectGroundBitBorderR);
			effect_flg |= (exist_b ? 0 : SNWNearbyEffectGroundBitBorderB);
			effect_flg |= (exist_l ? 0 : SNWNearbyEffectGroundBitBorderL);
				}
	}

	// ボーダー追加設定 周りが高いとき
	if (!exist_top)
	{
		// ボーダーの反映
		effect_flg |= (exist_uu ? SNWNearbyEffectGroundBitBorderU : 0);
		effect_flg |= (exist_ru ? SNWNearbyEffectGroundBitBorderR : 0);
		effect_flg |= (exist_bu ? SNWNearbyEffectGroundBitBorderB : 0);
		effect_flg |= (exist_lu ? SNWNearbyEffectGroundBitBorderL : 0);
	}

	effect_flg |= (exist_u ? 0 : SNWNearbyEffectGroundBitBorderSideU);
	effect_flg |= (exist_r ? 0 : SNWNearbyEffectGroundBitBorderSideR);
	effect_flg |= (exist_b ? 0 : SNWNearbyEffectGroundBitBorderSideB);
	effect_flg |= (exist_l ? 0 : SNWNearbyEffectGroundBitBorderSideL);
	effect_flg |= (exist_bottom ? 0 : SNWNearbyEffectGroundBitBorderBottom);


	// 周囲と上に地形以外のセルがある
	if (!(exist_u && exist_r && exist_b && exist_l && exist_top))
	{
		// 影を作る地形があるか調べる
		if (JudgeGroundPShadow(x, y, z))
		{
			// 投影情報をセット
			effect_flg |= (exist_u || (EasyLight != SNWEasyLightDirBottom) ? 0 : SNWNearbyEffectGroundBitPShadowU);
			effect_flg |= (exist_r || (EasyLight != SNWEasyLightDirLeft) ? 0 : SNWNearbyEffectGroundBitPShadowR);
			effect_flg |= (exist_b || (EasyLight != SNWEasyLightDirUp) ? 0 : SNWNearbyEffectGroundBitPShadowB);
			effect_flg |= (exist_l || (EasyLight != SNWEasyLightDirRight) ? 0 : SNWNearbyEffectGroundBitPShadowL);
			effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowT);
		}
	}

	if (effect_flg != 0)
	{
		// エフェクトを登録する
		NearbySpace.RegisterGroundEffect(&obj_ptr->Pos, effect_flg);
	}

	return;
}

Boolean SNWorld::JudgeGroundPShadow(Int32 x, Int32 y, Int32 z)
{
	Boolean ret = false;
	Int32 step_x = 0;
	Int32 step_y = 0;
	Int32 step_z = 2;

	Int32 ref_x = x;
	Int32 ref_y = y;
	Int32 ref_z = z;

	switch (EasyLight)
	{
	case SNWEasyLightDirUp:
		step_x = 0;
		step_y = 1;
		break;
	case SNWEasyLightDirRight:
		step_x = -1;
		step_y = 0;
		break;
	case SNWEasyLightDirBottom:
		step_x = 0;
		step_y = -1;
		break;
	case SNWEasyLightDirLeft:
		step_x = 1;
		step_y = 0;
		break;
	}

	ref_x += step_x;
	ref_y += step_y;
	ref_z += 1;

	// 範囲内ぜんぶ見る(暫定)
	while (NearbySpace.RefSpace(ref_x, ref_y, ref_z))
	{
		ret = NearbySpace.IsBlocked(ref_x, ref_y, ref_z, SNWNearbyObjectTypeGround);
		ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z + 1, SNWNearbyObjectTypeGround);

		if (ret)
		{
			break;
		}

		ref_x += step_x;
		ref_y += step_y;
		ref_z += step_z;
	}

	return ret;
}

