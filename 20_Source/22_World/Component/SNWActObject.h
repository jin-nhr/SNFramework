#pragma once
#include "SNFrameworkInternal.h"
#include "SNWObjectBase.h"
#include "SNWObjectchip.h"

class SNWActObject
{
public:
    SNWActObject();

    virtual ~SNWActObject();

    // オブジェクト設定
    virtual Void SetObject(SNWObjectBase* obj);

    // ObjectInfo参照
    virtual SNWObjectInfo* RefInfo();

    // オブジェクトコード設定
    virtual Void SetObjectCode(SNWObjectchip::WObjectCode code);

    // 有効化
    virtual Void SetEnable(Boolean enable);

    // 可視化
    virtual Void SetVisible(Boolean visible);

    // 重力影響設定
    virtual Void SetWeight(Boolean weight);

    // 座標設定
    virtual Void SetPos(SNWorldPos* pos);

    // Walk
    virtual Void Walk(SNWorldDir dir);

    // Jog
    virtual Void Jog(SNWorldDir dir);

    // Stop
    virtual Void Stop();

    // Jump
    virtual Void Jump(SNWorldDir dir);


private:
    SNWObjectBase* Object;
};

