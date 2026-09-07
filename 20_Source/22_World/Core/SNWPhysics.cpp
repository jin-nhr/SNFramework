#include "SNWPhysics.h"
#include "SNMath.h"


SNWPhysics::SNWPhysics()
{
	Space = nullptr;
	GlobalObject = nullptr;

	return;
}

SNWPhysics::~SNWPhysics()
{
	return;
}

// 初期化
Void SNWPhysics::Initialize()
{
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

	obj = GlobalObject->RefObject(SNWGlobalObjectPlayer);

	info = obj->RefInfo();

	info->Pos.X += info->Speed.X;
	info->Pos.Y += info->Speed.Y;

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

