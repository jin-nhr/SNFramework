#include "SNWGroundMesh.h"
#include "SNMapchip.h"
#include "SNMath.h"
#include "SNSystemConfig.h"
#include "SNWindowsAPI.h"

SNWGroundMesh::SNWGroundMesh()
{
	// メッシュIDクリア
	MeshX = 0;
	MeshY = 0;
	MeshZ = 0;

	// パス設定
	GroundData.SetFolderPath(SNSystemConfig::WorldMeshPath);

	return;
}

SNWGroundMesh::~SNWGroundMesh()
{
	return;
}

// 初期化
Void SNWGroundMesh::Initialize()
{
	// ファイル名設定
	SetFileName();

	// メッシュデータクリア
	ClearMesh();

	return;
}

// 終了
Void SNWGroundMesh::Terminate()
{
	// 処理中だったら終了を待つ
	GroundData.WaitForOperationComplete();

	return;
}

// MeshID設定
Void SNWGroundMesh::SetMeshID(Int32 x, UInt32 y, UInt32 z)
{
	MeshX = x;
	MeshY = y;
	MeshZ = z;

	SetFileName();

	return;
}

// Clear
Void SNWGroundMesh::ClearMesh()
{
	SNWGroundMeshData* data_adr;

	if (!IsProc())
	{
		// データ領域確保
		GroundData.GetMemoryObject()->Allocate(sizeof(SNWGroundMeshData));
		GroundData.GetMemoryObject()->Clear();

		// アドレス取得
		data_adr = (SNWGroundMeshData*)GroundData.GetMemoryObject()->GetAddress();

		// タグ設定
		data_adr->Tag[0] = SNSystemConfig::WorldMeshFileTag[0];
		data_adr->Tag[1] = SNSystemConfig::WorldMeshFileTag[1];
		data_adr->Tag[2] = SNSystemConfig::WorldMeshFileTag[2];
		data_adr->Tag[3] = SNSystemConfig::WorldMeshFileTag[3];
	}

	return;
}


// Load
Void SNWGroundMesh::LoadMesh()
{
	if (!IsProc())
	{
		GroundData.Read();
	}

	return;
}

// Save
Void SNWGroundMesh::SaveMesh()
{
	if (!IsProc())
	{
		GroundData.Write();
	}

	return;
}

// コード取得
SNMapchip::SNMapchipCode SNWGroundMesh::GetCode(Int32 x, Int32 y, Int32 z)
{
	SNWGroundMeshData* data_adr;

	// アドレス取得
	data_adr = (SNWGroundMeshData*)GroundData.GetMemoryObject()->GetAddress();

	return (SNMapchip::SNMapchipCode)data_adr->Block[z][y][x];
}

// Code書き込み
Void SNWGroundMesh::SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code)
{
	SNWGroundMeshData* data_adr;

	// アドレス取得
	data_adr = (SNWGroundMeshData*)GroundData.GetMemoryObject()->GetAddress();

	data_adr->Block[z][y][x] = code;

	return;
}

// ファイルアクセス中？
Boolean SNWGroundMesh::IsProc()
{
	Boolean ret = false;

	if (!GroundData.IsOperationComplete())
	{
		ret = true;
	}

	return ret;
}

// データ有効？
Boolean SNWGroundMesh::IsValidData()
{
	Boolean ret = false;
	SNWGroundMeshData* data_adr;

	// アドレス取得
	data_adr = (SNWGroundMeshData*)GroundData.GetMemoryObject()->GetAddress();

	if ((data_adr->Tag[0] == SNSystemConfig::WorldMeshFileTag[0]) &&
		(data_adr->Tag[1] == SNSystemConfig::WorldMeshFileTag[1]) &&
		(data_adr->Tag[2] == SNSystemConfig::WorldMeshFileTag[2]) &&
		(data_adr->Tag[3] == SNSystemConfig::WorldMeshFileTag[3]))
	{
		ret = true;
	}

	return ret;
}


Void SNWGroundMesh::SetFileName()
{
	SNMemory str;

	str.Allocate(MAX_PATH * sizeof(Char));

	wsprintf((LPWSTR)str.GetAddress(), (LPWSTR)SNSystemConfig::WorldMeshFileName, MeshX, MeshY, MeshZ);

	GroundData.SetFolderFileName((String)str.GetAddress());

	return;
}
