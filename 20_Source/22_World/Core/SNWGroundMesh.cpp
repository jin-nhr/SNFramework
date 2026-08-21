#include "SNWGroundMesh.h"
#include "SNMapchip.h"
#include "SNMath.h"
#include "SNSystemConfig.h"
#include "SNWindowsAPI.h"
#include "SNAutoResource.h"

SNWGroundMesh::SNWGroundMesh()
{
	// メッシュIDクリア
	MeshX = 0;
	MeshY = 0;
	MeshZ = 0;

	SetCodeInfo = {0};

	State = SNWGroundMeshStateIdle;

	CS.Initialize();

	return;
}

SNWGroundMesh::~SNWGroundMesh()
{
	return;
}

// 初期化
Void SNWGroundMesh::Initialize()
{
	// 平面一枚分のリストを確保しておく
	BlockList.Allocate(sizeof(SNWGroundMeshFileData));
	BlockList.Clear();

	InitBlockList();

	return;
}

// 終了
Void SNWGroundMesh::Terminate()
{
	// 処理中だったら終了を待つ
	SNThread::WaitForThreadEnd();

	return;
}

// MeshID設定
Void SNWGroundMesh::SetMeshID(Int32 x, Int32 y, Int32 z)
{
	MeshX = x;
	MeshY = y;
	MeshZ = z;

	return;
}

// Clear
Void SNWGroundMesh::ClearMesh()
{
	if (!IsProc())
	{
		SNAutoResource res(&CS);
		InitBlockList();
	}

	return;
}


// Load
Void SNWGroundMesh::LoadMesh()
{
	if (!IsProc())
	{
		State = SNWGroundMeshStateRead;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

// Save
Void SNWGroundMesh::SaveMesh()
{
	if (!IsProc())
	{
		State = SNWGroundMeshStateWrite;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

// ブロック取得
SNWGroundMeshFileData* SNWGroundMesh::GetBlock()
{
	CS.Lock();
	return (SNWGroundMeshFileData*)BlockList.GetAddress();
}

// ブロック解放
Void SNWGroundMesh::ReleaseBlock()
{
	CS.Unlock();
	return;
}

// Code書き込み
Void SNWGroundMesh::SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code)
{
	if (!IsProc())
	{
		State = SNWGroundMeshStateSetCode;

		SetCodeInfo.X = (UInt16)x;
		SetCodeInfo.Y = (UInt16)y;
		SetCodeInfo.Z = (UInt16)z;
		SetCodeInfo.Code = (UInt16)code;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

// ファイルアクセス中？
Boolean SNWGroundMesh::IsProc()
{
	Boolean ret = false;

	// スレッド実行中またはIdle以外のとき
	if (GetRunStatus() || State != SNWGroundMeshStateIdle)
	{
		ret = true;
	}

	return ret;
}

Void SNWGroundMesh::InitBlockList()
{
	// アドレス取得
	SNWGroundMeshFileData* data_adr = (SNWGroundMeshFileData*)BlockList.GetAddress();

	// タグ設定
	data_adr->Tag[0] = SNSystemConfig::WorldMeshFileTag[0];
	data_adr->Tag[1] = SNSystemConfig::WorldMeshFileTag[1];
	data_adr->Tag[2] = SNSystemConfig::WorldMeshFileTag[2];
	data_adr->Tag[3] = SNSystemConfig::WorldMeshFileTag[3];

	data_adr->BlockNum = 0;

	return;
}


Void SNWGroundMesh::UserMain()
{
	switch (State)
	{
	case SNWGroundMeshStateRead:
		ReadMeshFile();
		break;

	case SNWGroundMeshStateWrite:
		WriteMeshFile();
		break;
	case SNWGroundMeshStateSetCode:
		SetCodeAsync();
		break;
	}

	State = SNWGroundMeshStateIdle;

	return;
}

Void SNWGroundMesh::ReadMeshFile()
{
	SNFile file;

	// ファイル情報設定
	SetFileName(&file);

	// ファイル読み込み
	file.Read();

	// 完了待ち
	file.WaitForOperationComplete();

	// ファイルチェック
	if (IsValidFile(&file))
	{
		SNAutoResource res(&CS);
		BlockList.Copy(file.GetDataAddress(), file.FileSize);
	}

	return;
}

Void SNWGroundMesh::WriteMeshFile()
{
	SNFile file;
	SNWGroundMeshFileData* data_adr = (SNWGroundMeshFileData*)BlockList.GetAddress();
	UInt32 file_size = 16 + sizeof(SNWGroundMeshBlockData) * data_adr->BlockNum;

	// ファイル情報設定
	SetFileName(&file);

	// メモリ確保 ヘッダ固定16+ブロック数
	file.GetMemoryObject()->Allocate(file_size);

	{
		SNAutoResource res(&CS);

		// 書き込みデータコピー
		file.GetMemoryObject()->Copy(data_adr, file_size);
	}

	// ファイル書き込み
	file.Write();

	// 完了待ち
	file.WaitForOperationComplete();

	return;
}

Void SNWGroundMesh::SetCodeAsync()
{
	Int64 cnt;
	SNWGroundMeshFileData* data_adr = (SNWGroundMeshFileData*)BlockList.GetAddress();


	for (cnt = 0; cnt < data_adr->BlockNum; cnt++)
	{
		// 同一座標のデータあり
		if ((data_adr->Block[cnt].X) == (SetCodeInfo.X) &&
			(data_adr->Block[cnt].Y) == (SetCodeInfo.Y) &&
			(data_adr->Block[cnt].Z) == (SetCodeInfo.Z))
		{
			SNAutoResource res(&CS);

			// コードがBlank=削除のときは最終データを持ってくる
			if (SetCodeInfo.Code == SNMapchip::SNMapchipBlank)
			{
				// 厳密な排他は行わないので最終データはそのまま残しておく
				data_adr->Block[cnt] = data_adr->Block[data_adr->BlockNum - 1];
				data_adr->BlockNum--;
			}
			else
			{
				// コードを上書きする
				data_adr->Block[cnt] = SetCodeInfo;
			}
			break;
		}
	}

	// 同一座標のブロックなし
	if (cnt >= data_adr->BlockNum)
	{
		if (data_adr->BlockNum < SNGroundMeshBlockDataSize - 1)
		{
			SNAutoResource res(&CS);

			data_adr->Block[cnt] = SetCodeInfo;
			data_adr->BlockNum++;
		}
	}


	return;
}


Void SNWGroundMesh::SetFileName(SNFile* file)
{
	SNMemory str;

	// パス設定
	file->SetFolderPath(SNSystemConfig::WorldMeshPath);

	// ファイル名設定
	str.Allocate(MAX_PATH * sizeof(Char));

	wsprintf((LPWSTR)str.GetAddress(), (LPWSTR)SNSystemConfig::WorldMeshFileName, MeshX, MeshY, MeshZ);

	return;
}

Boolean SNWGroundMesh::IsValidFile(SNFile* file)
{
	Boolean ret = false;
	SNWGroundMeshFileData* data;

	data = (SNWGroundMeshFileData*)file->GetDataAddress();

	if (data != nullptr)
	{
		if ((data->Tag[0] == SNSystemConfig::WorldMeshFileTag[0]) &&
			(data->Tag[1] == SNSystemConfig::WorldMeshFileTag[1]) &&
			(data->Tag[2] == SNSystemConfig::WorldMeshFileTag[2]) &&
			(data->Tag[3] == SNSystemConfig::WorldMeshFileTag[3]))
		{
			ret = true;
		}
	}

	return ret;
}
