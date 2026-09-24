#pragma once
#include "SNFrameworkInternal.h"
#include "SNWNearbySpace.h"
#include "SNWGlobalObject.h"
#include "SNWObjectchip.h"
#include "SNMapchip.h"

// 物理エンジン

class SNWPhysics
{
public:
	static constexpr Float32 SNReversPos = 0.02f;
	static constexpr Float32 SNRefPos = 0.03f;
	static constexpr Float32 SNMaxSpeed = 0.90f;
	static constexpr Float32 SNMinSpeed = 0.01f;
	static constexpr Float32 SNGravityGain = 1.0f;
	static constexpr Float32 SNGroundBlockSize = 1.0f;

	static constexpr Int32 SNPhysicsNearbyBlockMax = 49;

	enum SNPhysicsNearbyBlockIndex
	{
		SNPhysicsNearbyBlockIndexCenter,
		SNPhysicsNearbyBlockIndexFloor,
		SNPhysicsNearbyBlockIndexCeiling,
		SNPhysicsNearbyBlockIndexN,
		SNPhysicsNearbyBlockIndexS,
		SNPhysicsNearbyBlockIndexW,
		SNPhysicsNearbyBlockIndexE,
		SNPhysicsNearbyBlockIndexNum,
	};

	struct SNPhysicsNearbyBlockInfo
	{
		SNWNearbyObject* NearbyObj;
		Int32 Dist2;
	};

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


protected:
	// オブジェクトの速度更新
	virtual Void UpdateObjectSpeed(SNWObjectInfo* info);

	// オブジェクトの座標更新
	virtual Void UpdateObjectPos(SNWObjectInfo* info);

	// オブジェクト重複ブロック取得
	virtual Int32 GetNearbyBlockCenter(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist);

	// オブジェクト床面ブロック取得
	virtual Int32 GetNearbyBlockFloor(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist);

	// オブジェクト周辺ブロック取得
	virtual Int32 GetNearbyBlockFace(SNWObjectInfo* info, SNPhysicsNearbyBlockInfo* blocklist, Float32 from_x, Float32 to_x, Float32 from_y, Float32 to_y, Float32 from_z, Float32 to_z);

	// 足元ブロックコード取得
	virtual UInt16 GetFloorBlockCode();

	// 周辺ブロック衝突判定
	virtual Boolean NearbyBlockCollision(Int32 block_num, SNPhysicsNearbyBlockInfo* blocklist);

private:
	SNWNearbySpace* Space;
	SNWGlobalObject* GlobalObject;

	Int32 FloorBlockNum;
	SNPhysicsNearbyBlockInfo FloorBlockList[SNPhysicsNearbyBlockMax];
	UInt16 FloorBlockCode;

	Int32 CenterBlockNum;
	SNPhysicsNearbyBlockInfo CenterBlockList[SNPhysicsNearbyBlockMax];

};
