#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"
#include "SNSystemConfig.h"
#include "SNWObjectBase.h"

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
	SNWNearbyObjectTypeObjectShadow,	// オブジェクトの影
	SNWNearbyObjectTypeActiveObject,	// 動的オブジェクト
	SNWNearbyObjectTypeGround,			// 地形
	SNWNearbyObjectTypeFocus,			// フォーカス
	SNWNearbyObjectTypeNum
};

// 周辺オブジェクト
struct SNWNearbyObject
{
	SNWNearbyObjectType Type;
	SNWorldPos          Pos;		// セル登録位置
	Void*               UserData;	// 種別に応じたデータをセットする
	UInt64              Effect[SNWorldDirNum];
	SNWorldPos			Size;		// オブジェクトサイズ
	SNWorldPos			Segment;	// セグメントNo All=0が本体
	Float32				Dist2;
};

// SNWNearbyObjectTypeGround
// - UInt16 地形Code
// - UInt64 Effect関連Bit
enum SNWNearbyEffectGroundBit
{
	// グローバル光源による影
	SNWNearbyEffectGroundBitGShadowBase  = 0x00000001,

	// 他地形の投影
	SNWNearbyEffectGroundBitPShadowT = 0x00000040,

	SNWNearbyEffectGroundBitPShadowTUR = 0x00000100,
	SNWNearbyEffectGroundBitPShadowTBR = 0x00000200,
	SNWNearbyEffectGroundBitPShadowTBL = 0x00000400,
	SNWNearbyEffectGroundBitPShadowTUL = 0x00000800,

	// 段差境界
	SNWNearbyEffectGroundBitBorderU = 0x00001000,
	SNWNearbyEffectGroundBitBorderR = 0x00002000,
	SNWNearbyEffectGroundBitBorderB = 0x00004000,
	SNWNearbyEffectGroundBitBorderL = 0x00008000,

	SNWNearbyEffectGroundBitBorderSideR = 0x00020000,
	SNWNearbyEffectGroundBitBorderSideL = 0x00080000,

	SNWNearbyEffectGroundBitBorderBottom = 0x00100000,

	// グローバル光源による影
	SNWNearbyEffectGroundBitGShadowLLow = 0x01000000,
	SNWNearbyEffectGroundBitGShadowRLow = 0x02000000,
	SNWNearbyEffectGroundBitGShadowLMid = 0x04000000,
	SNWNearbyEffectGroundBitGShadowRMid = 0x08000000,
	SNWNearbyEffectGroundBitGShadowLHigh = 0x10000000,
	SNWNearbyEffectGroundBitGShadowRHigh = 0x20000000,
};

// SNWNearbyObjectTypeActiveObject
// - SNWObjectBaseのポインタ

// SNWNearbyObjectTypeFocus
// - 未使用



// 周辺空間セル
static constexpr Int32 SNWNearbySpaceObjectNum = 4;

struct SNWNearbySpaceCell
{
	UInt32 TimeStampG;
	UInt32 TimeStampO[SNWNearbySpaceObjectNum];
	SNWNearbyObject* ObjectG;
	SNWNearbyObject* ObjectO[SNWNearbySpaceObjectNum];
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
	virtual Void RegisterGroundData(SNWorldPos* glb_pos, UInt16 code, UInt64* effect);

	// オブジェクト登録
	virtual Void RegisterGObjectData(SNWObjectBase* obj);

	// 地形エフェクト登録(周辺空間)
	// true:変化あり、false:変化なし
	virtual Boolean RegisterGroundEffectToSpace(SNWNearbyObject* obj_ptr, SNWorldDir glb_light, UInt64 effect);

	virtual Void RegisterFocus(SNWorldPos* local_pos);

	// 床面検索
	virtual Void SearchFloorPos(SNWorldPos* center_pos, SNWorldPos* out_pos);

	// グローバルオブジェクトの影登録
	virtual Void RegisterGObjectShadow(SNWObjectBase* obj, SNWorldPos* floor_pos);

	// オブジェクト距離更新
	virtual Void UpdateObjectDist2();

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
	virtual SNWNearbyObject* RefObjectG(Int32 x, Int32 y, Int32 z);
	virtual Boolean RefObjectO(Int32 x, Int32 y, Int32 z, SNWNearbyObject** out_ptr, Int32 obj_num);

	// 周辺空間アクセス
	virtual SNWNearbySpaceCell* RefSpace(Int32 x, Int32 y, Int32 z);

	// 指定ブロックが指定種別か
	virtual Boolean IsBlocked(Int32 x, Int32 y, Int32 z, SNWNearbyObjectType type);

	// 影対象ブロックか判定
	virtual Boolean IsShadowBlocked(Int32 x, Int32 y, Int32 z);

	// 光源ブロックか判定
	virtual Boolean IsLightBlocked(Int32 x, Int32 y, Int32 z);

	// 遮蔽ブロック確認
	virtual Boolean ExistShadowBlock(Int32 from_x, Int32 from_y, Int32 from_z, Int32 to_x, Int32 to_y, Int32 to_z);

	// 経路探索
	virtual Boolean SearchRoute(Int32 from_x, Int32 from_y, Int32 from_z, Int32 to_x, Int32 to_y, Int32 to_z);

	// 周辺空間内か判定する
	virtual Boolean CollisionCellVSSpace(SNWorldPos* cell_pos);


private:
	static constexpr Float32 SNJinView = 10000;	// 神視点距離
	static constexpr Float32 SNShadowHeight = 1.0f;

	UInt32 TimeStamp;
	SNWorldPos BasePos;
	SNWNearbyObject Object[SNWNearbyObjectNum];
	UInt32 ObjectNum;
	SNList ObjectList;
	SNWNearbySpaceCell NearbySpace[SNWNearbySpaceSizeZ][SNWNearbySpaceSizeY][SNWNearbySpaceSizeX];
};


