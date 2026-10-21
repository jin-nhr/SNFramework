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


// ObjectInfoŽQÆ
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

// —LŒø‰»
Void SNWActObject::SetEnable(Boolean enable)
{
	SNWObjectInfo* info = RefInfo();
	
	if (info != nullptr)
	{
		info->Enable = enable;
	}

	return;
}

// ‰ÂŽ‹‰»
Void SNWActObject::SetVisible(Boolean visible)
{
	SNWObjectInfo* info = RefInfo();

	if (info != nullptr)
	{
		info->Visible = visible;
	}

	return;
}

// À•WÝ’è
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
Void SNWActObject::Walk(SNWorldDir dir, Int8 angle)
{
	SNWObjectEventParam param = {0};

	if (Object != nullptr)
	{
		param.Event = SNWObjectEventWalk;
		param.Param1 = dir;
		param.Param2 = angle;
		Object->Notify(&param);
	}

	return;
}

// Stop
Void SNWActObject::Stop()
{
	SNWObjectEventParam param = { 0 };

	if (Object != nullptr)
	{
		param.Event = SNWObjectEventStop;
		Object->Notify(&param);
	}

	return;
}

// Jump
Void SNWActObject::Jump()
{
	SNWObjectEventParam param = { 0 };

	if (Object != nullptr)
	{
		param.Event = SNWObjectEventJump;
		Object->Notify(&param);
	}

	return;
}

// JumpEnd
Void SNWActObject::JumpEnd()
{
	SNWObjectEventParam param = { 0 };

	if (Object != nullptr)
	{
		param.Event = SNWObjectEventJumpEnd;
		Object->Notify(&param);
	}

	return;
}

// Elevation
Void SNWActObject::Elevation(Boolean up)
{
	SNWObjectEventParam param = { 0 };

	if (Object != nullptr)
	{
		param.Event = SNWObjectEventElevation;
		param.Param1 = (UInt8)up;
		Object->Notify(&param);
	}

	return;
}


