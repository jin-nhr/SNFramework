#include "SNWActObject.h"
#include "SNWorld.h"

SNWActObject::SNWActObject()
{
	Object = nullptr;

	return;
}

SNWActObject::~SNWActObject()
{
	return;
}

Void SNWActObject::SetObject(SNWObjectBase* obj)
{
	Object = obj;

	return;
}


// ObjectInfo参照
SNWObjectInfo* SNWActObject::RefInfo()
{
	SNWObjectInfo* ret = nullptr;

	ret = Object->RefInfo();

	return ret;
}

Void SNWActObject::SetObjectCode(SNWObjectchip::WObjectCode code)
{
	SNWObjectInfo* info = RefInfo();

	if (info != nullptr)
	{
		info->Code = code;
	}

	return;
}

// 有効化
Void SNWActObject::SetEnable(Boolean enable)
{
	SNWObjectInfo* info = RefInfo();
	
	if (info != nullptr)
	{
		info->Enable = enable;
	}

	return;
}

// 可視化
Void SNWActObject::SetVisible(Boolean visible)
{
	SNWObjectInfo* info = RefInfo();

	if (info != nullptr)
	{
		info->Visible = visible;
	}

	return;
}

// 重力影響設定
Void SNWActObject::SetWeight(Boolean weight)
{
	SNWObjectInfo* info = RefInfo();

	if (info != nullptr)
	{
		info->Weight = weight;
	}

	return;
}

// 座標設定
Void SNWActObject::SetPos(SNWorldPos* pos)
{
	SNWObjectInfo* info = RefInfo();

	if (info != nullptr)
	{
		info->Pos = *pos;
	}

	return;
}

// Walk
Void SNWActObject::Walk(SNWorldDir dir)
{

}

// Jog
Void SNWActObject::Jog(SNWorldDir dir)
{

}

// Stop
Void SNWActObject::Stop()
{

}

// Jump
Void SNWActObject::Jump(SNWorldDir dir)
{

}
