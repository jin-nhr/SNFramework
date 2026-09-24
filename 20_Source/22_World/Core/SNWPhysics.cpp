#include "SNWPhysics.h"
#include "SNMath.h"
#include "SNWindowsAPI.h"

SNWPhysics::SNWPhysics()
{
	Int32 cnt;

	Space = nullptr;
	GlobalObject = nullptr;

	FloorBlockNum = 0;
	FloorBlockCode = 0;
	CenterBlockNum = 0;

	for (cnt = 0; cnt < SNPhysicsNearbyBlockMax; cnt++)
	{
		FloorBlockList[cnt].NearbyObj = nullptr;
		FloorBlockList[cnt].Dist2 = 0x7FFFFFFF;
		CenterBlockList[cnt].NearbyObj = nullptr;
		CenterBlockList[cnt].Dist2 = 0x7FFFFFFF;
	}

	return;
}

SNWPhysics::~SNWPhysics()
{
	return;
}

// 初期化
Void SNWPhysics::Initialize()
{
	Int32 cnt;

	Space = nullptr;
	GlobalObject = nullptr;

	FloorBlockNum = 0;
	FloorBlockCode = 0;
	CenterBlockNum = 0;

	for (cnt = 0; cnt < SNPhysicsNearbyBlockMax; cnt++)
	{
		FloorBlockList[cnt].NearbyObj = nullptr;
		FloorBlockList[cnt].Dist2 = 0x7FFFFFFF;
		CenterBlockList[cnt].NearbyObj = nullptr;
		CenterBlockList[cnt].Dist2 = 0x7FFFFFFF;
	}

	return;
}

// 終了
Void SNWPhysics::Terminate()
{
	return;
}

// 更新
Void SNWPhysics::Update()
{
	SNWObjectBase* obj;
	SNWObjectInfo* info;

	// 今はPC1個だけを対象に処理しているが
	// 最終的には全オブジェクトを対象に処理するように変更する


	obj = GlobalObject->RefObject(SNWGlobalObjectPlayer);

	info = obj->RefInfo();

	if (info->Enable)
	{
		// 床面ブロック取得
		FloorBlockNum = GetNearbyBlockFloor(info, FloorBlockList);

		// 速度更新
		UpdateObjectSpeed(info);

		// 位置更新
		UpdateObjectPos(info);
	}

	return;
}

// 周辺空間設定
Void SNWPhysics::SetSpace(SNWNearbySpace* space)
{
	Space = space;

	return;
}

// グローバルオブジェクト設定
Void SNWPhysics::SetGlobalObject(SNWGlobalObject* gobj)
{
	GlobalObject = gobj;

	return;
}

// オブジェクトの速度更新
Void SNWPhysics::UpdateObjectSpeed(SNWObjectInfo* info)
{
	UInt16 floor_block_code = GetFloorBlockCode();

	info->PhysicsAcc.X = info->Acceleration.X;
	info->Acceleration.X = 0;

	info->PhysicsAcc.Y = info->Acceleration.Y;
	info->Acceleration.Y = 0;

	info->PhysicsAcc.Z += (info->Acceleration.Z * SNMapchip::Data[floor_block_code].JumpScale);
	info->Acceleration.Z = 0;	// 加速度反映後クリア

	if (info->Weight)
	{
		info->PhysicsAcc.Z -= (SNGravityGain * SNSystemConfig::FPS / 1000.0f) * SNMapchip::Data[floor_block_code].FallScale;
	}

	info->Speed.X += (info->PhysicsAcc.X - info->Speed.X) * SNMapchip::Data[floor_block_code].MoveScale;
	info->Speed.Y += (info->PhysicsAcc.Y - info->Speed.Y) * SNMapchip::Data[floor_block_code].MoveScale;
	info->Speed.Z += (info->PhysicsAcc.Z - info->Speed.Z);

	// 極小の値は0にまるめる
	if (SNMath::AbsF(info->Speed.X) < SNMinSpeed)
	{
		info->Speed.X = 0;
	}

	if (SNMath::AbsF(info->Speed.Y) < SNMinSpeed)
	{
		info->Speed.Y = 0;
	}

	if (SNMath::AbsF(info->Speed.Z) < SNMinSpeed)
	{
		info->Speed.Z = 0;
	}

	// 最大速度を超えている場合は最大速度にまるめる
	info->Speed.X = SNMath::SaturateF(info->Speed.X, -1 * SNMaxSpeed, SNMaxSpeed);
	info->Speed.Y = SNMath::SaturateF(info->Speed.Y, -1 * SNMaxSpeed, SNMaxSpeed);
	info->Speed.Z = SNMath::SaturateF(info->Speed.Z, -1 * SNMaxSpeed, SNMaxSpeed);

	return;
}

// オブジェクトの座標更新
Void SNWPhysics::UpdateObjectPos(SNWObjectInfo* info)
{
	SNWObjectInfo obj_info = *info;
	SNWorldPos pos;
	Int32 from_x;
	Int32 from_y;
	Int32 from_z;
	Int32 to_x;
	Int32 to_y;
	Int32 to_z;
	Boolean col = false;

	SNWObjectInfo z_test_obj;
	SNPhysicsNearbyBlockInfo z_test_list[SNPhysicsNearbyBlockMax];
	Int32 z_test_num;
	Boolean z_test_col;

	// 移動前の周辺空間座標計算
	pos =
	{
		obj_info.Pos.X - Space->GetBasePos()->X,
		obj_info.Pos.Y - Space->GetBasePos()->Y,
		obj_info.Pos.Z - Space->GetBasePos()->Z
	};

	from_x = SNMath::FloorToInt(pos.X);
	from_y = SNMath::FloorToInt(pos.Y);
	from_z = SNMath::FloorToInt(pos.Z);

	// 座標更新
	obj_info.Pos.X += obj_info.Speed.X;
	obj_info.Pos.Y += obj_info.Speed.Y;
	obj_info.Pos.Z += obj_info.Speed.Z;

	// 移動後の周辺空間座標計算
	pos =
	{
		obj_info.Pos.X - Space->GetBasePos()->X,
		obj_info.Pos.Y - Space->GetBasePos()->Y,
		obj_info.Pos.Z - Space->GetBasePos()->Z
	};

	to_x = SNMath::FloorToInt(pos.X);
	to_y = SNMath::FloorToInt(pos.Y);
	to_z = SNMath::FloorToInt(pos.Z);

	// 移動先の重複ブロック取得
	CenterBlockNum = GetNearbyBlockCenter(&obj_info, CenterBlockList);

	// 移動先の侵入可否判定
	col = NearbyBlockCollision(CenterBlockNum, CenterBlockList);

	// Z移動なしの場合はZ+1をテストする
	if ((from_z == to_z) && col)
	{
		if (obj_info.Speed.Z == 0)
		{
			z_test_obj = obj_info;
			z_test_obj.Pos.Z += 1;
			z_test_num = GetNearbyBlockCenter(&z_test_obj, z_test_list);
			z_test_col = NearbyBlockCollision(z_test_num, z_test_list);
			if (!z_test_col)
			{
				// 衝突なしならこの結果を採用
				obj_info.Pos.Z += 1;
				col = z_test_col;
			}
		}
	}

	// X座標に変化あり
	if (from_x != to_x)
	{
		// 侵入不可
		if (col)
		{
			obj_info.Speed.X = 0;

			// 正方向
			if (from_x < to_x)
			{
				obj_info.Pos.X = Space->GetBasePos()->X + to_x - SNReversPos;
			}
			// 負方向
			else
			{
				obj_info.Pos.X = Space->GetBasePos()->X + from_x + SNReversPos;
			}
			to_x = from_x;
			
			// 侵入可否更新
			CenterBlockNum = GetNearbyBlockCenter(&obj_info, CenterBlockList);
			col = NearbyBlockCollision(CenterBlockNum, CenterBlockList);
		}

	}

	// Y座標に変化あり
	if (from_y != to_y)
	{
		// 侵入不可
		if (col)
		{
			obj_info.Speed.Y = 0;

			// 正方向
			if (from_y < to_y)
			{
				obj_info.Pos.Y = Space->GetBasePos()->Y + to_y - SNReversPos;
			}
			// 負方向
			else
			{
				obj_info.Pos.Y = Space->GetBasePos()->Y + from_y + SNReversPos;
			}
			to_y = from_y;
		}
	}

	// Z座標に変化あり
	if (from_z != to_z)
	{
		// 侵入不可
		if (col)
		{
			obj_info.PhysicsAcc.Z = 0;
			obj_info.Speed.Z = 0;
			// 正方向
			if (from_z < to_z)
			{
				obj_info.Pos.Z = Space->GetBasePos()->Z + to_z - SNReversPos;
			}
			// 負方向
			else
			{
				obj_info.Pos.Z = Space->GetBasePos()->Z + from_z + SNReversPos;
			}
			to_z = from_z;

			// 侵入可否更新
			CenterBlockNum = GetNearbyBlockCenter(&obj_info, CenterBlockList);
			col = NearbyBlockCollision(CenterBlockNum, CenterBlockList);
		}
	}

	// 更新した座標をセット
	info->Pos = obj_info.Pos;
	info->Speed = obj_info.Speed;
	info->PhysicsAcc = obj_info.PhysicsAcc;

	return;
}

// オブジェクト重複ブロック取得
Int32 SNWPhysics::GetNearbyBlockCenter(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist)
{
	SNWorldPos pos =
	{
		info->Pos.X - Space->GetBasePos()->X,
		info->Pos.Y - Space->GetBasePos()->Y,
		info->Pos.Z - Space->GetBasePos()->Z
	};
	Float32 from_x = pos.X + SNGroundBlockSize / 2.0f - (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 to_x = pos.X + SNGroundBlockSize / 2.0f + (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 from_y = pos.Y + SNGroundBlockSize / 2.0f - (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 to_y = pos.Y + SNGroundBlockSize / 2.0f + (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 from_z = pos.Z;
	Float32 to_z = pos.Z + SNWObjectchip::Data[info->Code].SizeV;

	return GetNearbyBlockFace(info, blocklist, from_x, to_x, from_y, to_y, from_z, to_z);
}

// オブジェクト床面ブロック取得
Int32 SNWPhysics::GetNearbyBlockFloor(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist)
{
	SNWorldPos pos =
	{
		info->Pos.X - Space->GetBasePos()->X,
		info->Pos.Y - Space->GetBasePos()->Y,
		info->Pos.Z - Space->GetBasePos()->Z
	};
	Float32 from_x = pos.X + SNGroundBlockSize / 2.0f - (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 to_x = pos.X + SNGroundBlockSize / 2.0f + (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 from_y = pos.Y + SNGroundBlockSize / 2.0f - (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 to_y = pos.Y + SNGroundBlockSize / 2.0f + (SNWObjectchip::Data[info->Code].SizeH / 2.0f);
	Float32 from_z = pos.Z;
	Float32 to_z = pos.Z + SNWObjectchip::Data[info->Code].SizeV;

	return GetNearbyBlockFace(info, blocklist, from_x, to_x, from_y, to_y, from_z - SNRefPos, from_z - SNRefPos);
}


Int32 SNWPhysics::GetNearbyBlockFace(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist, Float32 from_x, Float32 to_x, Float32 from_y, Float32 to_y, Float32 from_z, Float32 to_z)
{
	Int32 int_from_x = SNMath::FloorToInt(from_x);
	Int32 int_to_x = SNMath::FloorToInt(to_x);
	Int32 int_from_y = SNMath::FloorToInt(from_y);
	Int32 int_to_y = SNMath::FloorToInt(to_y);
	Int32 int_from_z = SNMath::FloorToInt(from_z);
	Int32 int_to_z = SNMath::FloorToInt(to_z);

	Int32 cnt_x;
	Int32 cnt_y;
	Int32 cnt_z;
	Int32 num = 0;

	Float32 delta_x;
	Float32 delta_y;
	Float32 delta_z;

	SNWNearbyObject* obj = nullptr;
	SNPhysicsNearbyBlockInfo* blk_inf;

	for (cnt_z = int_from_z; cnt_z <= int_to_z; cnt_z++)
	{
		for (cnt_y = int_from_y; cnt_y <= int_to_y; cnt_y++)
		{
			for (cnt_x = int_from_x; cnt_x <= int_to_x; cnt_x++)
			{
				if (Space->IsBlocked(cnt_x, cnt_y, cnt_z, SNWNearbyObjectTypeGround) && (num < SNPhysicsNearbyBlockMax))
				{
					// IsBlockedにより有効ブロックであることが確定している
					obj = Space->RefObjectG(cnt_x, cnt_y, cnt_z);

					blk_inf = &blocklist[num];
					blk_inf->NearbyObj = obj;

					delta_x = obj->Pos.X - info->Pos.X;
					delta_y = obj->Pos.Y - info->Pos.Y;
					delta_z = obj->Pos.Z - info->Pos.Z;

					blk_inf->Dist2 = (Int32)(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
					num++;
				}
			}
		}
	}

	return num;
}

UInt16 SNWPhysics::GetFloorBlockCode()
{
	Int32 cnt;
	Int32 dist2 = 0x7FFFFFFF;
	UInt16 ret = SNMapchip::SNMapchipBlank;

	for (cnt = 0; cnt < FloorBlockNum; cnt++)
	{
		// 近いブロックがあったらret更新
		if (dist2 > FloorBlockList[cnt].Dist2)
		{
			ret = (UInt16)(intptr_t)FloorBlockList[cnt].NearbyObj->UserData;
			dist2 = FloorBlockList[cnt].Dist2;
		}
	}

	return ret;
}

// 周辺ブロック衝突判定
Boolean SNWPhysics::NearbyBlockCollision(Int32 block_num, SNPhysicsNearbyBlockInfo* blocklist)
{
	Boolean ret = false;
	Int32 cnt;

	for (cnt = 0; cnt < block_num; cnt++)
	{
		if (SNMapchip::Data[(UInt16)(intptr_t)blocklist[cnt].NearbyObj->UserData].Collision)
		{
			ret = true;
			break;
		}

	}

	return ret;
}
