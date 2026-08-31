#include "SNWNearbySpace.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"

SNWNearbySpace::SNWNearbySpace()
{
	TimeStamp = 0;
	BasePos = {0};
	ZeroMemory(Object, sizeof(Object));
	ZeroMemory(NearbySpace, sizeof(NearbySpace));
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

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;

		// 空間登録
		cell_ptr = &NearbySpace[(Int32)obj_ptr->Pos.Z][(Int32)obj_ptr->Pos.Y][(Int32)obj_ptr->Pos.X];

		cell_ptr->Object = obj_ptr;
		cell_ptr->TimeStamp = TimeStamp;
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

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;

		// 空間登録
		cell_ptr = &NearbySpace[(Int32)obj_ptr->Pos.Z][(Int32)obj_ptr->Pos.Y][(Int32)obj_ptr->Pos.X];

		cell_ptr->Object = obj_ptr;
		cell_ptr->TimeStamp = TimeStamp;
	}

	return;
}

// 地形エフェクト登録
// エフェクトは当たり判定不要なので周辺空間に登録不要(というか地形と重複するので登録不可)
Void SNWNearbySpace::RegisterGroundEffect(SNWorldPos* local_pos, UInt64 effect)
{
	SNListContainer* it = nullptr;
	SNWNearbyObject* obj_ptr = nullptr;

	if (ObjectNum < SNWNearbyObjectNum)
	{
		// オブジェクト設定
		obj_ptr = &Object[ObjectNum];
		ObjectNum++;

		obj_ptr->Type = SNWNearbyObjectTypeEffectGround;

		// ローカル座標設定
		obj_ptr->Pos = *local_pos;

		obj_ptr->UserData = (Void*)effect;

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;
	}
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

		// リスト登録
		it = ObjectList.InsertLast();
		it->UserData = (Void*)obj_ptr;
	}
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

SNWNearbyObject* SNWNearbySpace::RefObject(Int32 x, Int32 y, Int32 z)
{
	SNWNearbyObject* ret = nullptr;
	SNWNearbySpaceCell* cell;

	cell = RefSpace(x, y, z);
	if ((cell != nullptr) && (cell->Object != nullptr) && (cell->TimeStamp == TimeStamp))
	{
		ret = cell->Object;
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

	top = RefObject(x, y, z);
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
