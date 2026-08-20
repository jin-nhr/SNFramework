#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNMapchip.h"

// 地形メッシュ


struct SNWGroundMeshData
{
	Char		Tag[4];
	UInt8		Block[SNWGroundMeshSizeZ][SNWGroundMeshSizeY][SNWGroundMeshSizeX];
};


class SNWGroundMesh
{
public:
	SNWGroundMesh();

	virtual ~SNWGroundMesh();

	// 初期化
	virtual Void Initialize();

	// 終了
	virtual Void Terminate();

	// MeshID設定
	virtual Void SetMeshID(Int32 x, UInt32 y, UInt32 z);

	// ClearMesh
	virtual Void ClearMesh();

	// Load
	virtual Void LoadMesh();

	// Save
	virtual Void SaveMesh();

	// Code取得
	virtual SNMapchip::SNMapchipCode GetCode(Int32 x, Int32 y, Int32 z);

	// Code書き込み
	virtual Void SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code);

	// ファイルアクセス中？
	virtual Boolean IsProc();

	// データ有効？
	virtual Boolean IsValidData();

protected:
	// ファイル名更新
	virtual Void SetFileName();

private:
	Int32 MeshX;
	Int32 MeshY;
	Int32 MeshZ;

	SNFile GroundData;

};


