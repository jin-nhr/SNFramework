#include "SNWNearbySpace.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"

SNWNearbySpace::SNWNearbySpace()
{
	TimeStamp = 0;
	BasePos = {0};
	ZeroMemory((Void*)Object, sizeof(Object));
	ZeroMemory((Void*)NearbySpace, sizeof(NearbySpace));
	ObjectNum = 0;

	return;

}
SNWNearbySpace::~SNWNearbySpace()
{
	return;
}

Void SNWNearbySpace::Initialize()
{
	// リスト確保
	ObjectList.Allocate(SNWNearbyObjectNum);

	return;
}

Void SNWNearbySpace::Terminate()
{
	ObjectList.Free();
	return;
}

// 基点座標更新
Void SNWNearbySpace::UpdateStart(SNWorldPos* cur_pos, UInt32 world_time)
{
	TimeStamp = world_time;

	// 基点を計算
	BasePos.X = (Float32)SNMath::FloorToInt(cur_pos->X - SNSystemConfig::WorldNearbySpaceSizeH);
	BasePos.Y = (Float32)SNMath::FloorToInt(cur_pos->Y - SNSystemConfig::WorldNearbySpaceSizeH);
	BasePos.Z = (Float32)SNMath::FloorToInt(cur_pos->Z - SNSystemConfig::WorldNearbySpaceSizeV);

	ObjectNum = 0;
	ObjectList.Clear();

	return;
}

// 地形データ登録
Void SNWNearbySpace::RegisterGroundData(SNWorldPos* glb_pos, UInt16 code)
{
	SNListContainer* it = nullptr;
	SNWNearbyObject* obj_ptr = nullptr;
	SNWNearbySpaceCell* cell_ptr = nullptr;

	if (ObjectNum < SNWNearbyObjectNum)
	{
		// オブジェクト設定
		obj_ptr = &Object[ObjectNum];
		ObjectNum++;

		obj_ptr->Type = SNWNearbyObjectTypeGround;

		// ローカル座標に変換
		obj_ptr->Pos.X = glb_pos->X - BasePos.X;
		obj_ptr->Pos.Y = glb_pos->Y - BasePos.Y;
		obj_ptr->Pos.Z = glb_pos->Z - BasePos.Z;

		obj_ptr->UserData = (Void*)code;
		obj_ptr->Effect = 0;
		obj_ptr->RealPos = obj_ptr->Pos;
		obj_ptr->KeyPos = obj_ptr->Pos;
		obj_ptr->Size.X = 1;
		obj_ptr->Size.Y = 1;
		obj_ptr->Size.Z = 1;

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;

		// 空間登録
		cell_ptr = &NearbySpace[(Int32)obj_ptr->Pos.Z][(Int32)obj_ptr->Pos.Y][(Int32)obj_ptr->Pos.X];

		cell_ptr->ObjectG = obj_ptr;
		cell_ptr->TimeStampG = TimeStamp;
	}
	return;
}

// オブジェクト登録
Void SNWNearbySpace::RegisterGObjectData(SNWObjectBase* obj)
{
	SNListContainer* it = nullptr;
	SNWNearbyObject* obj_ptr = nullptr;
	SNWNearbySpaceCell* cell_ptr = nullptr;
	SNWObjectInfo* obj_info = obj->RefInfo();

	if (ObjectNum < SNWNearbyObjectNum)
	{
		// オブジェクト設定
		obj_ptr = &Object[ObjectNum];
		ObjectNum++;

		obj_ptr->Type = SNWNearbyObjectTypeActiveObject;

		// ローカル座標に変換
		obj_ptr->Pos.X = obj_info->Pos.X - BasePos.X;
		obj_ptr->Pos.Y = obj_info->Pos.Y - BasePos.Y;
		obj_ptr->Pos.Z = obj_info->Pos.Z - BasePos.Z;

		obj_ptr->UserData = (Void*)obj;
		obj_ptr->Effect = 0;
		obj_ptr->RealPos = obj_ptr->Pos;
		obj_ptr->KeyPos = obj_ptr->Pos;
		obj_ptr->Size.X = SNWObjectchip::Data[obj->GetCode()].SizeX;
		obj_ptr->Size.Y = SNWObjectchip::Data[obj->GetCode()].SizeY;
		obj_ptr->Size.Z = SNWObjectchip::Data[obj->GetCode()].SizeZ;
		obj_ptr->KeyPos.X = SNMath::FloorToInt(obj_ptr->KeyPos.X + (0.5f + obj_ptr->Size.X / 2));
		obj_ptr->KeyPos.Y = SNMath::FloorToInt(obj_ptr->KeyPos.Y + (0.5f + obj_ptr->Size.Y / 2));

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;

		// 空間登録
		cell_ptr = &NearbySpace[(Int32)obj_ptr->Pos.Z][(Int32)obj_ptr->Pos.Y][(Int32)obj_ptr->Pos.X];

		cell_ptr->ObjectO = obj_ptr;
		cell_ptr->TimeStampO = TimeStamp;
	}

	return;
}


Void SNWNearbySpace::RegisterGroundEffectToSpace(SNWNearbyObject* obj_ptr, UInt64 effect)
{
	obj_ptr->Effect = effect;

	return;
}


// フォーカス登録
Void SNWNearbySpace::RegisterFocus(SNWorldPos* glb_pos)
{
	SNListContainer* it = nullptr;
	SNWNearbyObject* obj_ptr = nullptr;

	if (ObjectNum < SNWNearbyObjectNum)
	{
		// オブジェクト設定
		obj_ptr = &Object[ObjectNum];
		ObjectNum++;

		obj_ptr->Type = SNWNearbyObjectTypeFocus;

		// ローカル座標に変換
		obj_ptr->Pos.X = glb_pos->X - BasePos.X;
		obj_ptr->Pos.Y = glb_pos->Y - BasePos.Y;
		obj_ptr->Pos.Z = glb_pos->Z - BasePos.Z;

		obj_ptr->UserData = 0;
		obj_ptr->Effect = 0;
		obj_ptr->RealPos = obj_ptr->Pos;
		obj_ptr->KeyPos = obj_ptr->Pos;
		obj_ptr->Size.X = 1;
		obj_ptr->Size.Y = 1;
		obj_ptr->Size.Z = 1;

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;
	}
	return;
}

// 床面検索
Void SNWNearbySpace::SearchFloorPos(SNWorldPos* center_pos, SNWorldPos* out_pos)
{
	// 中心座標が乗っている床面を検索
	Int32 x = SNMath::FloorToInt(center_pos->X - BasePos.X);
	Int32 y = SNMath::FloorToInt(center_pos->Y - BasePos.Y);
	Int32 z = SNMath::FloorToInt(center_pos->Z - BasePos.Z) - 1;
	SNWNearbySpaceCell* cell;
	SNWNearbyObject* obj_ptr;
	SNWObjectBase* base_ptr;
	Int32 cnt = 0;

	cell = RefSpace(x, y, z);

	while (RefSpace(x, y, z) && (cnt < SNSystemConfig::GroundPShadowSearchRange))
	{
		if (IsBlocked(x, y, z, SNWNearbyObjectTypeGround))
		{
			out_pos->X = (Float32)x;
			out_pos->Y = (Float32)y;
			out_pos->Z = (Float32)z;
			break;
		}

		// オブジェクト検索の方法は別途見当が必要かも
		else if (IsBlocked(x, y, z, SNWNearbyObjectTypeActiveObject))
		{
			obj_ptr = RefObjectO(x, y, z);
			base_ptr = (SNWObjectBase*)obj_ptr->UserData;
			out_pos->X = (Float32)x;
			out_pos->Y = (Float32)y;
			out_pos->Z = (Float32)(z + SNWObjectchip::Data[base_ptr->GetCode()].SizeZ);
			break;
		}

		z--;
		cnt++;
	}

	return;
}

// グローバルオブジェクトの影登録
Void SNWNearbySpace::RegisterGObjectShadow(SNWObjectBase* obj, SNWorldPos* floor_pos)
{
	SNListContainer* it = nullptr;
	SNWNearbyObject* obj_ptr = nullptr;

	if (ObjectNum < SNWNearbyObjectNum)
	{
		// オブジェクト設定
		obj_ptr = &Object[ObjectNum];
		ObjectNum++;

		obj_ptr->Type = SNWNearbyObjectTypeObjectShadow;

		// ローカル座標に変換
		obj_ptr->Pos.X = obj->RefInfo()->Pos.X - BasePos.X;
		obj_ptr->Pos.Y = obj->RefInfo()->Pos.Y - BasePos.Y;

		// 影が微妙に上下してしまうのを防ぐため
		// オブジェクトが地面から一定以上はなれたときだけ地面側の高さを採用する
		if (SNMath::AbsF((floor_pos->Z + 1) - obj->RefInfo()->Pos.Z - -BasePos.Z) < 0.5f)
		{
			obj_ptr->Pos.Z = obj->RefInfo()->Pos.Z - BasePos.Z;
		}
		else
		{
			obj_ptr->Pos.Z = floor_pos->Z + 1;
		}

		obj_ptr->UserData = (Void*)SNWorldShadowObject1;
		obj_ptr->Effect = 0;
		obj_ptr->RealPos = obj_ptr->Pos;
		obj_ptr->KeyPos = obj_ptr->Pos;
		obj_ptr->Size.X = SNWObjectchip::Data[obj->GetCode()].SizeX;
		obj_ptr->Size.Y = SNWObjectchip::Data[obj->GetCode()].SizeY;
		obj_ptr->Size.Z = SNWObjectchip::Data[obj->GetCode()].SizeZ;
		obj_ptr->KeyPos.X = SNMath::FloorToInt(obj_ptr->KeyPos.X + (0.5f + obj_ptr->Size.X / 2));
		obj_ptr->KeyPos.Y = SNMath::FloorToInt(obj_ptr->KeyPos.Y + (0.5f + obj_ptr->Size.Y / 2));

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;
	}

	return;
}

Void SNWNearbySpace::UpdateObjectPos(SNWNearbyObject* obj_ptr, SNWorldPos* pos)
{
	obj_ptr->RealPos = *pos;
	obj_ptr->KeyPos = *pos;

	obj_ptr->KeyPos.X = SNMath::FloorToInt(obj_ptr->KeyPos.X + (0.5f + obj_ptr->Size.X / 2));
	obj_ptr->KeyPos.Y = SNMath::FloorToInt(obj_ptr->KeyPos.Y + (0.5f + obj_ptr->Size.Y / 2));

	return;
}

// 起点座標取得
SNWorldPos* SNWNearbySpace::GetBasePos()
{
	return &BasePos;
}

UInt32 SNWNearbySpace::GetTimeStamp()
{
	return TimeStamp;
}

SNList* SNWNearbySpace::GetList()
{
	return &ObjectList;
}

// オブジェクト数
UInt32 SNWNearbySpace::GetObjectNum()
{
	return ObjectNum;
}

// オブジェクトアクセス
SNWNearbyObject* SNWNearbySpace::RefObject(Int32 index)
{
	return &Object[index];
}

SNWNearbyObject* SNWNearbySpace::RefObjectG(Int32 x, Int32 y, Int32 z)
{
	SNWNearbyObject* ret = nullptr;
	SNWNearbySpaceCell* cell;

	cell = RefSpace(x, y, z);
	if ((cell != nullptr) && (cell->ObjectG != nullptr) && (cell->TimeStampG == TimeStamp))
	{
		ret = cell->ObjectG;
	}

	return ret;
}

SNWNearbyObject* SNWNearbySpace::RefObjectO(Int32 x, Int32 y, Int32 z)
{
	SNWNearbyObject* ret = nullptr;
	SNWNearbySpaceCell* cell;

	cell = RefSpace(x, y, z);
	if ((cell != nullptr) && (cell->ObjectO != nullptr) && (cell->TimeStampO == TimeStamp))
	{
		ret = cell->ObjectO;
	}

	return ret;
}


// 周辺空間アクセス
SNWNearbySpaceCell* SNWNearbySpace::RefSpace(Int32 x, Int32 y, Int32 z)
{
	SNWNearbySpaceCell* ret = nullptr;

	if ((0 <= x) && (x < SNWNearbySpaceSizeX) &&
		(0 <= y) && (y < SNWNearbySpaceSizeY) &&
		(0 <= z) && (z < SNWNearbySpaceSizeZ))
	{
		ret = &NearbySpace[z][y][x];
	}

	return ret;
}

Boolean SNWNearbySpace::IsBlocked(Int32 x, Int32 y, Int32 z, SNWNearbyObjectType type)
{
	Boolean ret = false;
	SNWNearbyObject* top;

	top = RefObjectG(x, y, z);
	if ((top != nullptr) && (top->Type == type))
	{
		ret = true;
	}

	return ret;
}


Boolean SNWNearbySpace::CollisionCellVSSpace(SNWorldPos* cell_pos)
{
	Boolean ret = false;
	SNWorldPos* space_base_pos = GetBasePos();

	// X軸判定
	if ((cell_pos->X <= space_base_pos->X + SNWNearbySpaceSizeX - 1) &&
		(space_base_pos->X <= cell_pos->X) &&

		// Y軸判定
		(cell_pos->Y <= space_base_pos->Y + SNWNearbySpaceSizeY - 1) &&
		(space_base_pos->Y <= cell_pos->Y) &&

		// Z軸判定
		(cell_pos->Z <= space_base_pos->Z + SNWNearbySpaceSizeZ - 1) &&
		(space_base_pos->Z <= cell_pos->Z))
	{
		ret = true;
	}

	return ret;
}
