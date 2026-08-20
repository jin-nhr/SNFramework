#include "SNWGround.h"
#include "SNMath.h"
#include "SNSystemConfig.h"
#include "SNGraphicsDevice.h"
#include "SNGraphicsContext.h"
#include "SNGraphicsResManager.h"


SNWGround::SNWGround()
{
	Int32 dir;
	Int32 z;

	CurrentPos = { 0 };
	MeshExitPos = { 0 };
	WriteID = { 0 };

	// MeshInfo, MeshRef初期化
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			MeshInfo[z * SNWorldDirNum + dir].ID = { 0 };
			MeshInfo[z * SNWorldDirNum + dir].RefCount = 0;
			MeshInfo[z * SNWorldDirNum + dir].State = SNWMeshStateIdle;
			MeshInfo[z * SNWorldDirNum + dir].Dirty = false;

			MeshRef[z][dir] = nullptr;
		}
	}

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

	// MeshInfo, MeshRef初期化
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			CalcMeshID(&MeshExitPos, (SNWorldDir)dir, (SNWorldElevation)z, &MeshInfo[z * SNWorldDirNum + dir].ID);
			MeshInfo[z * SNWorldDirNum + dir].Mesh.Initialize();
			MeshInfo[z * SNWorldDirNum + dir].RefCount = 0;
			MeshInfo[z * SNWorldDirNum + dir].State = SNWMeshStateIdle;

			MeshRef[z][dir] = &MeshInfo[z * SNWorldDirNum + dir];
			MeshRef[z][dir]->RefCount++;

			// ロード指示
			MeshRef[z][dir]->Mesh.LoadMesh();
			MeshRef[z][dir]->State = SNWMeshStateLoad;
		}
	}

	return;
}

// 終了
Void SNWGround::Terminate()
{
	Int32 dir;
	Int32 z;

	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			MeshInfo[z * SNWorldDirNum + dir].Mesh.Terminate();
		}
	}
	return;
}

// 地形更新
Void SNWGround::Update(SNWorldPos* pos)
{
	Boolean is_idle;
	SNWorldPos center_id;

	CurrentPos = *pos;

	// 状態更新
	UpdateState();

	// 処理が終わっているかチェック
	is_idle = IsOpeComp();

	// 処理が終わっているときだけ更新チェック
	if (is_idle)
	{
		// メッシュ外判定
		if (JudgeOutMesh(pos, &MeshRef[SNWorldElevationMid][SNWorldDirCenter]->ID))
		{
			// 移動元ID計算
			CalcMeshCenterID(&MeshExitPos, &center_id);

			// 移動元IDが未保存
			if (!IsSameID(&WriteID, &center_id))
			{
				SaveMesh();
				WriteID = center_id;
			}

			// 保存済み
			else
			{
				// メッシュ更新処理
				UpdateMesh(pos);
			}
		}
		// メッシュ移動なし
		else
		{
			// 出口更新
			MeshExitPos = *pos;
		}
	}

	return;
}

// 地形描画
Void SNWGround::Draw(SNBitmap* target, SNWorldPos* target_pos, Boolean focus, SNWorldDir dir, SNWorldShadowDir shadow_dir, Int32 in_range, Int32 in_range_z)
{
	Int32 x_start;
	Int32 x_num;
	Int32 x_step;

	Int32 y_start;
	Int32 y_num;
	Int32 y_step;
	
	Int32 z_start;
	Int32 z_num;
	Int32 z_step;

	Int32 x_cnt;
	Int32 y_cnt;
	Int32 z_cnt;

	SNWorldDir out_dir;
	SNWorldElevation z;
	SNWorldPos lpos;

	SNWorldPos in_pos;

	UInt16 code;
	SNRect dst_rect;
	SNRect src_rect;

	SNMapchip::SNMapchipCode chip_code;

	SNSize dst_size;
	SNPoint lt_pos = { 0 };

	
	x_start = (Int32)(target_pos->X - in_range);
	x_step = 1;
	y_start = (Int32)(target_pos->Y - in_range);
	y_step = 1;
	z_start = (Int32)(target_pos->Z - in_range_z);
	z_step = 1;

	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	SNColor color = { 0, 0, 0, 0 };
	SNBitmap* source = SNGraphicsResManager::GetResource(SNGraphicsResMapchip1);

	// targetの中心座標から地形データ左上角の座標を算出
	target->GetSize(&dst_size);
	lt_pos.X =
		dst_size.Width / 2
		- SNMapchip::MapchipCenterOffset[dir].X
		- (SNMapchip::MapchipStrideX[dir].X * in_range
			+ SNMapchip::MapchipStrideY[dir].X * in_range
			+ SNMapchip::MapchipStrideZ[dir].X * in_range_z);
	lt_pos.Y = 
		dst_size.Height / 2 
		- SNMapchip::MapchipCenterOffset[dir].Y 
		- (SNMapchip::MapchipStrideX[dir].Y * in_range
			+ SNMapchip::MapchipStrideY[dir].Y * in_range
			+ SNMapchip::MapchipStrideZ[dir].Y * in_range_z);

	// ワークに対してテキスト描画
	grc->Begin(target);
	grc->Clear(&color);

	x_num = in_range * 2 + 1;
	y_num = x_num;
	z_num = in_range_z * 2 + 1;

	in_pos.X = (Float32)x_start;
	in_pos.Y = (Float32)y_start;
	in_pos.Z = (Float32)z_start;
	z_cnt = 0;

	while (z_cnt < z_num)
	{
		in_pos.Y = (Float32)y_start;
		y_cnt = 0;

		while (y_cnt < y_num)
		{
			in_pos.X = (Float32)x_start;
			x_cnt = 0;

			while (x_cnt < x_num)
			{
				// 方位等と座標取得
				if (CvtIDAndLocalPos(&in_pos, &out_dir, &z, &lpos))
				{
					chip_code = MeshRef[z][out_dir]->Mesh.GetCode((Int32)lpos.X, (Int32)lpos.Y, (Int32)lpos.Z);

					// マップチップ取得
					code = SNMapchip::Data[chip_code].Code;

					// チップ側の矩形取得
					SNMapchip::CodeToRect(code, dir, &src_rect);

					dst_rect.PointX = lt_pos.X
						+ (SNMapchip::MapchipStrideX[dir].X * x_cnt
							+ SNMapchip::MapchipStrideY[dir].X * y_cnt
							+ SNMapchip::MapchipStrideZ[dir].X * z_cnt);
					dst_rect.PointY = lt_pos.Y
						+ (SNMapchip::MapchipStrideX[dir].Y * x_cnt
							+ SNMapchip::MapchipStrideY[dir].Y * y_cnt
							+ SNMapchip::MapchipStrideZ[dir].Y * z_cnt);

					dst_rect.Width = src_rect.Width;
					dst_rect.Height = src_rect.Height;

					if (chip_code != SNMapchip::SNMapchipBlank)
					{
						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);


						// 影描画
						SNMapchip::CodeToRect(SNMapchip::ShadowCode[shadow_dir], dir, &src_rect);

						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);
					}

					// フォーカス
					if ((focus == true) && (x_cnt == y_cnt) && (x_cnt == in_range) && (z_cnt == in_range_z))
					{
						// 影描画
						SNMapchip::CodeToRect(SNMapchip::FocusCode, dir, &src_rect);

						grc->DrawImage(
							&dst_rect,
							source,
							&src_rect,
							SNAlphaMax);
					}
				}
				in_pos.X += x_step;
				x_cnt++;
			}
			in_pos.Y += y_step;
			y_cnt++;
		}
		in_pos.Z += z_step;
		z_cnt++;
	}

	grc->End();

	return;
}

// 地形書き込み
Void SNWGround::Write(SNMapchip::SNMapchipCode code)
{
	SNWorldDir dir;
	SNWorldElevation z;
	SNWorldPos lpos;

	// 方位等と座標取得
	if (CvtIDAndLocalPos(&CurrentPos, &dir, &z, &lpos))
	{
		MeshRef[z][dir]->Mesh.SetCode((Int32)lpos.X, (Int32)lpos.Y, (Int32)lpos.Z, code);
	}

	return;
}


// 処理完了判定
Boolean SNWGround::IsOpeComp()
{
	Boolean ret = true;
	Int32 dir;
	Int32 z;

	// 全部Idleかチェック
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (MeshRef[z][dir]->State != SNWMeshStateIdle)
			{
				ret = false;
				break;
			}
		}
		if (ret == false)
		{
			break;
		}
	}

	return ret;
}

// センターID計算
Void SNWGround::CalcMeshCenterID(SNWorldPos* cur_pos, SNWorldPos* out_id)
{
	out_id->X = (Float32)SNMath::FloorToInt(cur_pos->X / SNWGroundMeshSizeX);
	out_id->Y = (Float32)SNMath::FloorToInt(cur_pos->Y / SNWGroundMeshSizeY);
	out_id->Z = (Float32)SNMath::FloorToInt(cur_pos->Z / SNWGroundMeshSizeZ);

	return;
}


// MeshID計算
Void SNWGround::CalcMeshID(SNWorldPos* cur_pos, SNWorldDir dir, SNWorldElevation ele, SNWorldPos* out_id)
{
	SNWorldPos tmp_pos;

	CalcMeshCenterID(cur_pos, &tmp_pos);
	
	OffsetMeshID(&tmp_pos, dir, ele, out_id);

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

// メッシュ外判定
Boolean SNWGround::JudgeOutMesh(SNWorldPos* cur_pos, SNWorldPos* center_id)
{
	Boolean ret = false;
	SNWorldPos tmp_pos;

	CvtIDToPos(center_id, &tmp_pos);

	// X軸判定
	if ((cur_pos->X <= tmp_pos.X - SNWMeshLoadThresholdX) ||
		(tmp_pos.X + SNWGroundMeshSizeX + SNWMeshLoadThresholdX <= cur_pos->X))
	{
		ret = true;
	}

	// Y軸判定
	else if ((cur_pos->Y <= tmp_pos.Y - SNWMeshLoadThresholdY) ||
		(tmp_pos.Y + SNWGroundMeshSizeY + SNWMeshLoadThresholdY <= cur_pos->Y))
	{
		ret = true;
	}

	// Z軸判定
	else if ((cur_pos->Z <= tmp_pos.Z - SNWMeshLoadThresholdZ) ||
		(tmp_pos.Z + SNWGroundMeshSizeZ + SNWMeshLoadThresholdZ <= cur_pos->Z))
	{
		ret = true;
	}

	return ret;
}

Void SNWGround::UpdateMesh(SNWorldPos* cur_pos)
{
	Int32 dir;
	Int32 z;
	SNWorldPos center_id;
	SNWorldPos mesh_id;
	Int32 mesh_idx;

	CalcMeshCenterID(cur_pos, &center_id);

	// Mesh移動処理
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			// 対象位置のオフセット計算
			OffsetMeshID(&center_id, (SNWorldDir)dir, (SNWorldElevation)z, &mesh_id);

			// ID検索
			mesh_idx = SearchMesh(&mesh_id);

			MeshRef[z][dir]->RefCount--;
			MeshRef[z][dir] = nullptr;

			// 発見
			if (mesh_idx != -1)
			{
				MeshRef[z][dir] = &MeshInfo[mesh_idx];
				MeshRef[z][dir]->RefCount++;
			}
		}
	}

	// 更新対象Meshの処理
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (MeshRef[z][dir] == nullptr)
			{
				// 空きMesh検索
				mesh_idx = SearchMeshBlank();

				if (mesh_idx != -1)
				{
					// 対象位置のオフセット計算
					OffsetMeshID(&center_id, (SNWorldDir)dir, (SNWorldElevation)z, &mesh_id);

					MeshRef[z][dir] = &MeshInfo[mesh_idx];

					MeshRef[z][dir]->ID = mesh_id;
					MeshRef[z][dir]->Mesh.LoadMesh();
					MeshRef[z][dir]->State = SNWMeshStateLoad;
					MeshRef[z][dir]->RefCount++;
				}
			}
		}
	}

	return;
}

// メッシュ検索
Int32 SNWGround::SearchMesh(SNWorldPos* id)
{
	Int32 ret = -1;
	Int32 dir;
	Int32 z;

	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (IsSameID(&MeshInfo[z * SNWorldDirNum + dir].ID, id))
			{
				ret = z * SNWorldDirNum + dir;
				break;
			}
		}

		if (ret != -1)
		{
			break;
		}
	}

	return ret;
}

// メッシュ検索
Int32 SNWGround::SearchMeshBlank()
{
	Int32 ret = -1;
	Int32 dir;
	Int32 z;

	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (MeshInfo[z * SNWorldDirNum + dir].RefCount == 0)
			{
				ret = z * SNWorldDirNum + dir;
				break;
			}
		}

		if (ret != -1)
		{
			break;
		}
	}

	return ret;
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

	// ダーティなら書き込み指示
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (MeshRef[z][dir]->Dirty)
			{
				MeshRef[z][dir]->Mesh.SaveMesh();
				MeshRef[z][dir]->State = SNWMeshStateSave;
				MeshRef[z][dir]->Dirty = false;
			}
		}
	}

	return;
}

// 状態更新
Void SNWGround::UpdateState()
{
	Int32 dir;
	Int32 z;

	// 処理が終わってるものはIdleにする
	for (z = 0; z < SNWorldElevationNum; z++)
	{
		for (dir = 0; dir < SNWorldDirNum; dir++)
		{
			if (!MeshRef[z][dir]->Mesh.IsProc())
			{
				MeshRef[z][dir]->State = SNWMeshStateIdle;
			}
		}
	}

	return;
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

	SNWorldPos lu_pos;
	SNWorldPos delta;
	
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
	

	// Mesh左上座標取得
	CvtIDToPos(&MeshRef[SNWorldElevationLow][SNWorldDirNW]->ID, &lu_pos);

	// Offset計算
	delta.X = in_pos->X - lu_pos.X;
	delta.Y = in_pos->Y - lu_pos.Y;
	delta.Z = in_pos->Z - lu_pos.Z;

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
