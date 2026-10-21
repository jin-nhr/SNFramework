#pragma once
#include "SNFrameworkInternal.h"
#include "SNWObjectBase.h"
#include "SNWObjectchip.h"

// 動的オブジェクト

// オブジェクト実体はObjectBaseであり
// 動的なObjectBaseへのI/Fを提供するのがActObjectである
// そのためActObject自身はデータを持たない

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

    // 座標設定
    virtual Void SetPos(SNWorldPos* pos);

    // Walk
    // グローバル座標系における方向を指定する
    virtual Void Walk(SNWorldDir dir, Int8 angle);

    // Stop
    virtual Void Stop();

    // Jump
    virtual Void Jump();

    // Jump End
    virtual Void JumpEnd();

    // Elevation
    virtual Void Elevation(Boolean up);


private:
    SNWObjectBase* Object;
};

