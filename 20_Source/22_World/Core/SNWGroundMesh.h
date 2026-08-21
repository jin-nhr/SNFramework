#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNMapchip.h"
#include "SNThread.h"
#include "SNList.h"
#include "SNCriticalSection.h"

// 地形メッシュ


struct SNWGroundMeshBlockData
{
	UInt16 Code;
	UInt16 X;
	UInt16 Y;
	UInt16 Z;
};

// ブロック最大数を定義
// ブロックをしきめつめた面を3面(地面、天井、地下)分は表現できるようにしておく
static constexpr UInt32 SNGroundMeshBlockDataSize = SNWGroundMeshSizeX * SNWGroundMeshSizeY * 3;

struct SNWGroundMeshFileData
{
	Char		Tag[4];
	UInt32		BlockNum;
	SNWGroundMeshBlockData Block[SNGroundMeshBlockDataSize];
};


enum SNWGroundMeshState
{
	SNWGroundMeshStateIdle,
	SNWGroundMeshStateRead,
	SNWGroundMeshStateWrite,
	SNWGroundMeshStateSetCode,
};



class SNWGroundMesh : private SNThread
{
public:
	SNWGroundMesh();

	virtual ~SNWGroundMesh();

	// 初期化
	virtual Void Initialize();

	// 終了
	virtual Void Terminate();

	// MeshID設定
	virtual Void SetMeshID(Int32 x, Int32 y, Int32 z);

	// ClearMesh
	virtual Void ClearMesh();

	// Load
	virtual Void LoadMesh();

	// Save
	virtual Void SaveMesh();

	// ブロックデータ参照
	SNWGroundMeshFileData* GetBlock();
	Void ReleaseBlock();

	// Code書き込み
	virtual Void SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code);

	// ファイルアクセス中？
	virtual Boolean IsProc();

	// ブロックリスト初期設定
	virtual Void InitBlockList();

public:
	SNCriticalSection CS;	// ブロックリスト参照用

private:
	virtual Void UserMain();

	virtual Void ReadMeshFile();

	virtual Void WriteMeshFile();

	virtual Void SetCodeAsync();

	virtual Void SetFileName(SNFile* file);

	virtual Boolean IsValidFile(SNFile* file);

	SNWGroundMeshState State;

	Int32 MeshX;
	Int32 MeshY;
	Int32 MeshZ;

	SNWGroundMeshBlockData SetCodeInfo;

	SNMemory BlockList;
};
