#pragma once
#include "SNFrameworkInternal.h"
#include "SNWGroundMesh.h"
#include "SNBitmap.h"

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
	SNWGroundMesh	Mesh;
	SNWMeshState	State;
	Int32			RefCount;
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

	// 地形描画
	virtual Void Draw(SNBitmap* target, SNWorldPos* target_pos, Boolean focus, SNWorldDir dir, SNWorldShadowDir shadow_dir, Int32 in_range, Int32 in_range_z);

	// 地形書き込み
	virtual Void Write(SNMapchip::SNMapchipCode code);


protected:

	// 処理完了判定
	virtual Boolean IsOpeComp();

	// 中央のメッシュID計算
	virtual Void CalcMeshCenterID(SNWorldPos* cur_pos, SNWorldPos* out_id);

	// メッシュID計算
	virtual Void CalcMeshID(SNWorldPos* cur_pos, SNWorldDir dir, SNWorldElevation ele, SNWorldPos* out_id);

	// メッシュIDオフセット
	virtual Void OffsetMeshID(SNWorldPos* cur_pos, SNWorldDir dir, SNWorldElevation ele, SNWorldPos* out_id);

	// メッシュ外判定
	virtual Boolean JudgeOutMesh(SNWorldPos* cur_pos, SNWorldPos* center_id);

	// メッシュ移動
	virtual Void UpdateMesh(SNWorldPos* cur_pos);

	// メッシュ検索
	virtual Int32 SearchMesh(SNWorldPos* id);

	// メッシュの空き検索
	virtual Int32 SearchMeshBlank();

	// ID同一判定
	virtual Boolean IsSameID(SNWorldPos* id1, SNWorldPos* id2);

	// Mesh保存
	virtual Void SaveMesh();

	// 状態更新
	virtual Void UpdateState();

	// MeshID→座標変換
	virtual Void CvtIDToPos(SNWorldPos* id, SNWorldPos* pos);

	// 座標→ID＋ローカル座標
	// 範囲外:false
	virtual Boolean CvtIDAndLocalPos(SNWorldPos* in_pos, SNWorldDir* out_dir, SNWorldElevation* out_z, SNWorldPos* out_pos);

private:
	SNWorldPos CurrentPos;
	SNWorldPos MeshExitPos;
	SNWorldPos WriteID;

	SNWMeshInfo	MeshInfo[SNWorldElevationNum * SNWorldDirNum];
	SNWMeshInfo* MeshRef[SNWorldElevationNum][SNWorldDirNum];
};


