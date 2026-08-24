#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"
#include "SNSystemConfig.h"

// 周辺空間


constexpr UInt32 SNWNearbyObjectUserDataSize = 64;
constexpr UInt32 SNWNearbyObjectNum = 65536 * 4;

constexpr UInt32 SNWNearbySpaceSizeX = SNSystemConfig::WorldNearbySpaceSizeH * 2 + 1;
constexpr UInt32 SNWNearbySpaceSizeY = SNSystemConfig::WorldNearbySpaceSizeH * 2 + 1;
constexpr UInt32 SNWNearbySpaceSizeZ = SNSystemConfig::WorldNearbySpaceSizeV * 2 + 1;

// オブジェクト種別
enum SNWNearbyObjectType
{
	SNWNearbyObjectTypeUnknown,
	SNWNearbyObjectTypeGround,			// 地形
	SNWNearbyObjectTypeEffectGround,	// 地形エフェクト
	SNWNearbyObjectTypeFocus,			// フォーカス
	SNWNearbyObjectTypeNum
};

// 周辺オブジェクト
struct SNWNearbyObject
{
	SNWNearbyObjectType Type;
	SNWorldPos          Pos;
	Void*               UserData;	// 種別に応じたデータをセットする
};

// SNWNearbyObjectTypeGround
// - UInt16 地形Code

// SNWNearbyObjectTypeEffectGround
// - UInt64 Effect関連Bit
enum SNWNearbyEffectGroundBit
{
	// グローバル光源による影
	SNWNearbyEffectGroundBitGShadowU = 0x00000001,
	SNWNearbyEffectGroundBitGShadowR = 0x00000002,
	SNWNearbyEffectGroundBitGShadowB = 0x00000004,
	SNWNearbyEffectGroundBitGShadowL = 0x00000008,

	// 他地形の投影
	SNWNearbyEffectGroundBitPShadowU = 0x00000010,
	SNWNearbyEffectGroundBitPShadowR = 0x00000020,
	SNWNearbyEffectGroundBitPShadowB = 0x00000040,
	SNWNearbyEffectGroundBitPShadowL = 0x00000080,
	SNWNearbyEffectGroundBitPShadowT = 0x00000100,

	// 段差境界
	SNWNearbyEffectGroundBitBorderU = 0x00001000,
	SNWNearbyEffectGroundBitBorderR = 0x00002000,
	SNWNearbyEffectGroundBitBorderB = 0x00004000,
	SNWNearbyEffectGroundBitBorderL = 0x00008000,

	SNWNearbyEffectGroundBitBorderSideU = 0x00010000,
	SNWNearbyEffectGroundBitBorderSideR = 0x00020000,
	SNWNearbyEffectGroundBitBorderSideB = 0x00040000,
	SNWNearbyEffectGroundBitBorderSideL = 0x00080000,

	SNWNearbyEffectGroundBitBorderBottom = 0x00100000,

};    

// 周辺空間セル
struct SNWNearbySpaceCell
{
	UInt32 TimeStamp;
	SNWNearbyObject* Object;
};

class SNWNearbySpace
{
public:
	SNWNearbySpace();
	virtual ~SNWNearbySpace();

	virtual Void Initialize();
	virtual Void Terminate();

	// 起点座標更新
	virtual Void UpdateStart(SNWorldPos* cur_pos, UInt32 world_time);

	// 地形データ登録
	virtual Void RegisterGroundData(SNWorldPos* glb_pos, UInt16 code);

	// 地形エフェクト登録
	virtual Void RegisterGroundEffect(SNWorldPos* local_pos, UInt64 effect);

	virtual Void RegisterFocus(SNWorldPos* local_pos);

	// 起点座標取得
	virtual SNWorldPos* GetBasePos();

	// タイムスタンプ取得
	virtual UInt32 GetTimeStamp();

	// リスト取得
	virtual SNList* GetList();

	// オブジェクト数
	virtual UInt32 GetObjectNum();

	// オブジェクトアクセス
	virtual SNWNearbyObject* RefObject(Int32 index);
	virtual SNWNearbyObject* RefObject(Int32 x, Int32 y, Int32 z);

	// 周辺空間アクセス
	virtual SNWNearbySpaceCell* RefSpace(Int32 x, Int32 y, Int32 z);

	// 指定ブロックが指定種別か
	virtual Boolean IsBlocked(Int32 x, Int32 y, Int32 z, SNWNearbyObjectType type);


private:
	UInt32 TimeStamp;
	SNWorldPos BasePos;
	SNWNearbyObject Object[SNWNearbyObjectNum];
	UInt32 ObjectNum;
	SNList ObjectList;
	SNWNearbySpaceCell NearbySpace[SNWNearbySpaceSizeZ][SNWNearbySpaceSizeY][SNWNearbySpaceSizeX];
};


