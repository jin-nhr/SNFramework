#pragma once
#include "SNFrameworkInternal.h"
#include "SNWGroundMesh.h"
#include "SNList.h"

// 地形クラス

enum SNWMeshState
{
	SNWMeshStateIdle,
	SNWMeshStateLoad,
	SNWMeshStateSave,
	SNWMeshStateNum,
};


struct SNWMeshInfo
{
	SNWorldPos		ID;
	SNWorldPos		MeshPos;
	SNWGroundMesh	Mesh;
	SNWMeshState	State;
	Boolean			Dirty;
};

class SNWGround
{
public:
	SNWGround();
	virtual ~SNWGround();

	// 初期化
	virtual Void Initialize();

	// 終了
	virtual Void Terminate();

	// 地形更新
	virtual Void Update(SNWorldPos* pos);

	// 地形書き込み
	virtual Void Write(SNMapchip::SNMapchipCode code);


protected:
	// 座標→ID変換
	virtual Void CvtPosToID(SNWorldPos* cur_pos, SNWorldPos* out_id);

	// メッシュIDオフセット
	virtual Void OffsetMeshID(SNWorldPos* cur_pos, SNWorldDir dir, SNWorldElevation ele, SNWorldPos* out_id);

	// メッシュ更新判定
	// 現在座標をもとに周辺メッシュの更新判定を行う
	virtual Boolean JudgeUpdateMesh();

	// メッシュ更新
	virtual Void UpdateMesh();

	// メッシュ移動量計算
	virtual Void CalcMeshMoveDelta(SNWorldPos* delta_id);

	// メッシュ移動処理
	virtual Void MoveMesh(SNWorldPos* delta_id);
	virtual Void MoveMeshXY(Int32 cnt, UInt16* array1, UInt16* array2, UInt16* array3);
	virtual Void MoveMeshZ(Int32 cnt, UInt16 idx1, UInt16 idx2, UInt16 idx3);
	virtual Void MoveMesh1(UInt16 from_z, UInt16 from_dir, UInt16 to_z, UInt16 to_dir);

	// メッシュロード処理
	virtual Void LoadMesh();

	// ID同一判定
	virtual Boolean IsSameID(SNWorldPos* id1, SNWorldPos* id2);

	// Mesh保存
	virtual Void SaveMesh();

	// 状態更新
	virtual Boolean UpdateState();

	// MeshID→座標変換
	virtual Void CvtIDToPos(SNWorldPos* id, SNWorldPos* pos);

	// 座標→ID＋ローカル座標
	// 範囲外:false
	virtual Boolean CvtIDAndLocalPos(SNWorldPos* in_pos, SNWorldDir* out_dir, SNWorldElevation* out_z, SNWorldPos* out_pos);


private:
	SNWorldPos CurrentPos;
	SNWorldPos CurrentID;
	SNWorldPos CenterPos;
	SNWorldPos CenterID;
	SNWorldPos SavedCenterID;

	SNWMeshInfo	MeshInfo[SNWorldElevationNum * SNWorldDirNum];
	Int16 MeshRef[SNWorldElevationNum][SNWorldDirNum];

	SNList MeshPool;
};


