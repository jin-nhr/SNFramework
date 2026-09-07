#pragma once
#include "SNFrameworkInternal.h"
#include "SNWNearbySpace.h"
#include "SNWGlobalObject.h"

// 物理エンジン

class SNWPhysics
{
public:
	SNWPhysics();

	virtual ~SNWPhysics();

	// 初期化
	virtual Void Initialize();

	// 終了
	virtual Void Terminate();

	// 更新
	virtual Void Update();

	// 周辺空間設定
	virtual Void SetSpace(SNWNearbySpace* space);

	// グローバルオブジェクト設定
	virtual Void SetGlobalObject(SNWGlobalObject* gobj);


private:
	SNWNearbySpace* Space;
	SNWGlobalObject* GlobalObject;
};
