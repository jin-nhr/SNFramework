#pragma once
#include "SNFrameworkInternal.h"
#include "SNFile.h"
#include "SNMapchip.h"
#include "SNThread.h"
#include "SNList.h"
#include "SNCriticalSection.h"
#include "SNSystemConfig.h"

// 地形メッシュ

// ブロック最大数を定義
// ブロックをしきめつめた面を3面(地面、天井、地下)分は表現できるようにしておく
static constexpr UInt32 SNGroundBlockDataSize = SNWGroundMeshSizeX * SNWGroundMeshSizeY * 3;



struct SNWGroundBlockData
{
	UInt16 Code;
	UInt16 X;
	UInt16 Y;
	UInt16 Z;
};

struct SNWGroundFileData
{
	Char		Tag[4];
	UInt32		BlockNum;
	SNWGroundBlockData Block[SNGroundBlockDataSize];
};


class SNWGround : private SNThread
{
public:
	SNWGround();

	virtual ~SNWGround();

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
	SNWGroundFileData* GetBlock();
	Void ReleaseBlock();

	// Code書き込み
	virtual Void SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code);

	// Code書き込み実行
	virtual Void RunSetCode();

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

	SNWMeshState State;

	Int32 MeshX;
	Int32 MeshY;
	Int32 MeshZ;

	SNWGroundBlockData SetCodeInfo[SNSystemConfig::WorldNearbySpaceSizeH * SNSystemConfig::WorldNearbySpaceSizeH * SNSystemConfig::WorldNearbySpaceSizeV];
	Int32 SetCodeInfoNum;

	SNMemory BlockList;
};
