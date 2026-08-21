#include "SNWGround.h"
#include "SNMath.h"
#include "SNSystemConfig.h"
#include "SNGraphicsDevice.h"
#include "SNGraphicsContext.h"
#include "SNGraphicsResManager.h"
#include "SNWindowsAPI.h"


SNWGround::SNWGround()
{
	Int32 dir;
	Int32 z;
	SNWMeshInfo* mesh_info;

	CurrentPos = { 0 };
	CurrentID = { 0 };
	CenterPos = { 0 };
	CenterID = { 0 };
	SavedCenterID = { 0 };

	// MeshInfo, MeshRef初期化
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			mesh_info = &MeshInfo[z * SNWorldDirNum + dir];

			mesh_info->ID = { 0 };
			mesh_info->MeshPos = { 0 };
			mesh_info->State = SNWMeshStateIdle;
			mesh_info->Dirty = false;

			MeshRef[z][dir] = -1;
		}
	}

	// 空きMesh用のプール確保
	MeshPool.Allocate(SNWorldElevationNum * SNWorldDirNum);

	return;

}
SNWGround::~SNWGround()
{

	return;
}

// 初期化
Void SNWGround::Initialize()
{
	Int32 dir;
	Int32 z;
	SNWMeshInfo* mesh_info;

	// MeshInfo, MeshRef初期化
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			mesh_info = &MeshInfo[z * SNWorldDirNum + dir];

			OffsetMeshID(&CurrentID, (SNWorldDir)dir, (SNWorldElevation)z, &mesh_info->ID);
			CvtIDToPos(&mesh_info->ID, &mesh_info->MeshPos);
			mesh_info->Mesh.Initialize();
			mesh_info->Mesh.SetMeshID((UInt32)mesh_info->ID.X, (UInt32)mesh_info->ID.Y, (UInt32)mesh_info->ID.Z);
			mesh_info->State = SNWMeshStateIdle;

			MeshRef[z][dir] = (Int16)(z * SNWorldDirNum + dir);

			// ロード指示
			mesh_info->Mesh.LoadMesh();
			mesh_info->State = SNWMeshStateLoad;
		}
	}

	return;
}

// 終了
Void SNWGround::Terminate()
{
	Int32 dir;
	Int32 z;
	SNWMeshInfo* mesh_info;

	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			mesh_info = &MeshInfo[z * SNWorldDirNum + dir];
			mesh_info->Mesh.Terminate();
		}
	}
	return;
}

// 地形更新
Void SNWGround::Update(SNWorldPos* pos)
{
	Boolean is_idle;

	CurrentPos = *pos;
	CvtPosToID(&CurrentPos, &CurrentID);

	// 状態更新
	is_idle = UpdateState();

	// 処理が終わっているときだけ更新チェック
	if (is_idle)
	{
		// メッシュ更新判定
		if (JudgeUpdateMesh())
		{
			// 現在のMeshが未保存？
			if (!IsSameID(&SavedCenterID, &CenterID))
			{
				SaveMesh();
				SavedCenterID = CenterID;
			}

			// 保存済み
			else
			{
				// メッシュ更新処理
				UpdateMesh();
			}
		}
		// メッシュ更新なし
		else
		{
			// 中央座標更新
			CenterPos = *pos;
			CvtPosToID(&CenterPos, &CenterID);
		}
	}

	return;
}

// 地形書き込み
Void SNWGround::Write(SNMapchip::SNMapchipCode code)
{
	SNWorldDir dir;
	SNWorldElevation z;
	SNWorldPos lpos;

	// メッシュ方位とローカル座標取得
	if (CvtIDAndLocalPos(&CurrentPos, &dir, &z, &lpos))
	{
		MeshInfo[MeshRef[z][dir]].Mesh.SetCode((Int32)lpos.X, (Int32)lpos.Y, (Int32)lpos.Z, code);
		MeshInfo[MeshRef[z][dir]].Dirty = true;
	}

	return;
}

// 座標→ID変換
Void SNWGround::CvtPosToID(SNWorldPos* cur_pos, SNWorldPos* out_id)
{
	out_id->X = (Float32)SNMath::FloorToInt(cur_pos->X / SNWGroundMeshSizeX);
	out_id->Y = (Float32)SNMath::FloorToInt(cur_pos->Y / SNWGroundMeshSizeY);
	out_id->Z = (Float32)SNMath::FloorToInt(cur_pos->Z / SNWGroundMeshSizeZ);

	return;
}

// メッシュIDオフセット
Void SNWGround::OffsetMeshID(SNWorldPos* cur_id, SNWorldDir dir, SNWorldElevation ele, SNWorldPos* out_id)
{
	const SNPoint offset[SNWorldDirNum] =
	{
		{	0,	0	},	// Center
		{	0,	-1	},	// N
		{	1,	-1	},	// NE
		{	1,	0	},	// E
		{	1,	1	},	// SE
		{	0,	1	},	// S
		{	-1,	1	},	// SW
		{	-1,	0	},	// W
		{	-1,	-1	},	// NW
	};

	const Int8 z_offset[SNWorldElevationNum] =
	{
		1,	// Up
		0,	// Mid
		-1,	// Low
	};

	out_id->X = cur_id->X + offset[dir].X;
	out_id->Y = cur_id->Y + offset[dir].Y;
	out_id->Z = cur_id->Z + z_offset[ele];

	return;
}

// メッシュ更新判定
// 中央メッシュ＋閾値の範囲をでたかどうかをチェック
Boolean SNWGround::JudgeUpdateMesh()
{
	Boolean ret = false;
	SNWorldPos* tmp_pos = &MeshInfo[MeshRef[SNWorldElevationMid][SNWorldDirCenter]].MeshPos;
	SNWorldPos* cur_pos = &CurrentPos;

	// X軸判定
	if ((cur_pos->X <= tmp_pos->X - SNWMeshLoadThresholdX) ||
		(tmp_pos->X + SNWGroundMeshSizeX + SNWMeshLoadThresholdX <= cur_pos->X))
	{
		ret = true;
	}

	// Y軸判定
	else if ((cur_pos->Y <= tmp_pos->Y - SNWMeshLoadThresholdY) ||
		(tmp_pos->Y + SNWGroundMeshSizeY + SNWMeshLoadThresholdY <= cur_pos->Y))
	{
		ret = true;
	}

	// Z軸判定
	else if ((cur_pos->Z <= tmp_pos->Z - SNWMeshLoadThresholdZ) ||
		(tmp_pos->Z + SNWGroundMeshSizeZ + SNWMeshLoadThresholdZ <= cur_pos->Z))
	{
		ret = true;
	}

	return ret;
}

// メッシュ更新
Void SNWGround::UpdateMesh()
{
	SNWorldPos delta_id;

	// 移動量計算
	CalcMeshMoveDelta(&delta_id);

	// 移動処理
	MoveMesh(&delta_id);

	// メッシュロード処理
	LoadMesh();

	return;
}

// メッシュ移動量計算
Void SNWGround::CalcMeshMoveDelta(SNWorldPos* delta_id)
{
	SNWorldPos* center_id = &MeshInfo[MeshRef[SNWorldElevationMid][SNWorldDirCenter]].ID;

	// 中央とカレントIDの差分計算
	delta_id->X = CurrentID.X - center_id->X;
	delta_id->Y = CurrentID.Y - center_id->Y;
	delta_id->Z = CurrentID.Z - center_id->Z;

	return;
}

Void SNWGround::MoveMesh(SNWorldPos* delta_id)
{
	// 1→2→3の方向に移動する
	
	// NW,  N, NE
	//  W,  C,  E
	// SW,  S, SE

	// コピー方向←
	if (delta_id->X < 0)
	{
		UInt16 array1[3] = { SNWorldDirNE, SNWorldDirE, SNWorldDirSE };
		UInt16 array2[3] = { SNWorldDirN, SNWorldDirCenter, SNWorldDirS };
		UInt16 array3[3] = { SNWorldDirNW, SNWorldDirW, SNWorldDirSW };

		MoveMeshXY((Int32)(delta_id->X * -1), array1, array2, array3);
	}
	// コピー方向→
	else if (0 < delta_id->X)
	{
		UInt16 array1[3] = { SNWorldDirNW, SNWorldDirW, SNWorldDirSW };
		UInt16 array2[3] = { SNWorldDirN, SNWorldDirCenter, SNWorldDirS };
		UInt16 array3[3] = { SNWorldDirNE, SNWorldDirE, SNWorldDirSE };

		MoveMeshXY((Int32)(delta_id->X), array1, array2, array3);
	}
	// コピー方向↑
	if (delta_id->Y < 0)
	{
		UInt16 array1[3] = { SNWorldDirSW, SNWorldDirS, SNWorldDirSE };
		UInt16 array2[3] = { SNWorldDirW, SNWorldDirCenter, SNWorldDirE };
		UInt16 array3[3] = { SNWorldDirNW, SNWorldDirN, SNWorldDirNE };

		MoveMeshXY((Int32)(delta_id->Y * -1), array1, array2, array3);
	}
	// コピー方向↓
	else if (0 < delta_id->Y)
	{
		UInt16 array1[3] = { SNWorldDirNW, SNWorldDirN, SNWorldDirNE };
		UInt16 array2[3] = { SNWorldDirW, SNWorldDirCenter, SNWorldDirE };
		UInt16 array3[3] = { SNWorldDirSW, SNWorldDirS, SNWorldDirSE };

		MoveMeshXY((Int32)(delta_id->Y), array1, array2, array3);
	}
	// コピー方向 高→低
	if (delta_id->Z < 0)
	{
		UInt16 idx1 = SNWorldElevationUp;
		UInt16 idx2 = SNWorldElevationMid;
		UInt16 idx3 = SNWorldElevationLow;

		MoveMeshZ((Int32)(delta_id->Z * -1), idx1, idx2, idx3);
	}
	// コピー方向 低→高
	else if (0 < delta_id->Z)
	{
		UInt16 idx1 = SNWorldElevationLow;
		UInt16 idx2 = SNWorldElevationMid;
		UInt16 idx3 = SNWorldElevationUp;

		MoveMeshZ((Int32)(delta_id->Z), idx1, idx2, idx3);
	}

	return;
}

Void SNWGround::MoveMeshXY(Int32 cnt, UInt16* array1, UInt16* array2, UInt16* array3)
{
	UInt16 move_cnt;
	UInt16 z_cnt;
	UInt16 dir_cnt;

	// 移動数ループ
	for (move_cnt = 0; move_cnt < cnt; move_cnt++)
	{
		// Zループ
		for (z_cnt = 0; z_cnt < SNWorldElevationNum; z_cnt++)
		{
			// 3回ループ
			for (dir_cnt = 0; dir_cnt < 3; dir_cnt++)
			{
				MoveMesh1(z_cnt, array2[dir_cnt], z_cnt, array3[dir_cnt]);
				MoveMesh1(z_cnt, array1[dir_cnt], z_cnt, array2[dir_cnt]);
			}
		}
	}

	return;
}

Void SNWGround::MoveMeshZ(Int32 cnt, UInt16 idx1, UInt16 idx2, UInt16 idx3)
{
	UInt16 move_cnt;
	UInt16 dir_cnt;

	// 移動数ループ
	for (move_cnt = 0; move_cnt < cnt; move_cnt++)
	{
		// dirループ
		for (dir_cnt = 0; dir_cnt < SNWorldDirNum; dir_cnt++)
		{
			MoveMesh1(idx2, dir_cnt, idx3, dir_cnt);
			MoveMesh1(idx1, dir_cnt, idx2, dir_cnt);
		}
	}

	return;
}

Void SNWGround::MoveMesh1(UInt16 from_z, UInt16 from_dir, UInt16 to_z, UInt16 to_dir)
{
	SNListContainer* it;

	// 移動先をPoolに退避
	if (MeshRef[to_z][to_dir] != -1)
	{
		it = MeshPool.InsertLast();
		it->UserData = (Void*)MeshRef[to_z][to_dir];
	}

	// 移動
	MeshRef[to_z][to_dir] = MeshRef[from_z][from_dir];
	MeshRef[from_z][from_dir] = -1;

	return;
}

Void SNWGround::LoadMesh()
{
	Int32 dir;
	Int32 z;
	Int16 idx;
	SNWMeshInfo* mesh_info;
	SNListContainer* it;

	// 無効メッシュをロードする
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (MeshRef[z][dir] == -1)
			{
				// 空きを取得
				it = MeshPool.GetTop();
				if (it != nullptr)
				{
					idx = (Int16)(intptr_t)it->UserData;
					MeshPool.Remove(it);

					MeshRef[z][dir] = idx;
					mesh_info = &MeshInfo[idx];

					// ID, 座標を設定
					OffsetMeshID(&CurrentID, (SNWorldDir)dir, (SNWorldElevation)z, &mesh_info->ID);
					CvtIDToPos(&mesh_info->ID, &mesh_info->MeshPos);

					mesh_info->Mesh.SetMeshID((Int32)mesh_info->ID.X, (Int32)mesh_info->ID.Y, (Int32)mesh_info->ID.Z);

					// ロード指示
					mesh_info->Mesh.LoadMesh();
					mesh_info->State = SNWMeshStateLoad;
				}
			}
		}
	}

	return;
}

// ID同一判定
Boolean SNWGround::IsSameID(SNWorldPos* id1, SNWorldPos* id2)
{
	Boolean ret = false;

	if ((id1->X == id2->X) &&
		(id1->Y == id2->Y) &&
		(id1->Z == id2->Z))
	{
		ret = true;
	}

	return ret;
}

Void SNWGround::SaveMesh()
{
	Int32 dir;
	Int32 z;
	SNWMeshInfo* mesh_info;

	// ダーティなら書き込み指示
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			mesh_info = &MeshInfo[MeshRef[z][dir]];

			if (mesh_info->Dirty)
			{
				mesh_info->Mesh.SaveMesh();
				mesh_info->State = SNWMeshStateSave;
				mesh_info->Dirty = false;
			}
		}
	}

	return;
}

// 状態更新
Boolean SNWGround::UpdateState()
{
	Boolean ret = true;
	Int32 dir;
	Int32 z;
	SNWMeshInfo* mesh_info;

	// 処理が終わってるものはIdleにする
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			mesh_info = &MeshInfo[MeshRef[z][dir]];

			if (!mesh_info->Mesh.IsProc())
			{
				mesh_info->State = SNWMeshStateIdle;
			}
			else
			{
				// 処理中があればfalse
				ret = false;
			}
		}
	}

	return ret;
}

// ID座標変換
Void SNWGround::CvtIDToPos(SNWorldPos* id, SNWorldPos* pos)
{
	pos->X = id->X * SNWGroundMeshSizeX;
	pos->Y = id->Y * SNWGroundMeshSizeY;
	pos->Z = id->Z * SNWGroundMeshSizeZ;

	return;
}

// 座標→ID＋ローカル座標
// 範囲外:false
Boolean SNWGround::CvtIDAndLocalPos(SNWorldPos* in_pos, SNWorldDir* out_dir, SNWorldElevation* out_z, SNWorldPos* out_pos)
{
	Boolean ret = false;

	SNWorldPos* lu_pos;
	SNWorldPos delta = { 0 };
	
	Int32 id_x;
	Int32 id_y;
	Int32 id_z;

	const SNWorldDir dir_array[3][3] =
	{
		{SNWorldDirNW,		SNWorldDirN,			SNWorldDirNE		},
		{SNWorldDirW,		SNWorldDirCenter,		SNWorldDirE			},
		{SNWorldDirSW,		SNWorldDirS,			SNWorldDirSE		},
	};

	const SNWorldElevation z_array[3] =
	{
		SNWorldElevationLow,
		SNWorldElevationMid,
		SNWorldElevationUp,
	};
	

	// 周辺Mesh低＆左上座標取得
	lu_pos = &MeshInfo[MeshRef[SNWorldElevationLow][SNWorldDirNW]].MeshPos;

	// Offset計算
	delta.X = in_pos->X - lu_pos->X;
	delta.Y = in_pos->Y - lu_pos->Y;
	delta.Z = in_pos->Z - lu_pos->Z;

	id_x = SNMath::FloorToInt(delta.X / SNWGroundMeshSizeX);
	id_y = SNMath::FloorToInt(delta.Y / SNWGroundMeshSizeY);
	id_z = SNMath::FloorToInt(delta.Z / SNWGroundMeshSizeZ);

	if (((0 <= id_x) && (id_x < 3)) &&
		((0 <= id_y) && (id_y < 3)) &&
		((0 <= id_z) && (id_z < 3)))
	{

		*out_dir = dir_array[id_y][id_x];
		*out_z = z_array[id_z];

		out_pos->X = (Float32)((Int32)delta.X % SNWGroundMeshSizeX);
		out_pos->Y = (Float32)((Int32)delta.Y % SNWGroundMeshSizeY);
		out_pos->Z = (Float32)((Int32)delta.Z % SNWGroundMeshSizeZ);

		ret = true;
	}

	return ret;
}

