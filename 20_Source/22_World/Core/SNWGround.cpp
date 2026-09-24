#include "SNWGround.h"
#include "SNMapchip.h"
#include "SNMath.h"
#include "SNSystemConfig.h"
#include "SNWindowsAPI.h"
#include "SNAutoResource.h"
#include "SNWNearbySpace.h"

SNWGround::SNWGround()
{
	// メッシュIDクリア
	MeshX = 0;
	MeshY = 0;
	MeshZ = 0;

	ZeroMemory(SetCodeInfo, sizeof(SetCodeInfo));
	SetCodeInfoNum = 0;

	State = SNWMeshStateIdle;

	CS.Initialize();

	return;
}

SNWGround::~SNWGround()
{
	return;
}

// 初期化
Void SNWGround::Initialize()
{
	// 平面一枚分のリストを確保しておく
	BlockList.Allocate(sizeof(SNWGroundFileData));
	BlockList.Clear();

	InitBlockList();

	return;
}

// 終了
Void SNWGround::Terminate()
{
	// 処理中だったら終了を待つ
	SNThread::WaitForThreadEnd();

	return;
}

// MeshID設定
Void SNWGround::SetMeshID(Int32 x, Int32 y, Int32 z)
{
	MeshX = x;
	MeshY = y;
	MeshZ = z;

	return;
}

// Clear
Void SNWGround::ClearMesh()
{
	if (!IsProc())
	{
		SNAutoResource res(&CS);
		InitBlockList();
	}

	return;
}


// Load
Void SNWGround::LoadMesh()
{
	if (!IsProc())
	{
		State = SNWMeshStateRead;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

// Save
Void SNWGround::SaveMesh()
{
	if (!IsProc())
	{
		State = SNWMeshStateWrite;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

// ブロック取得
SNWGroundFileData* SNWGround::GetBlock()
{
	CS.Lock();
	return (SNWGroundFileData*)BlockList.GetAddress();
}

// ブロック解放
Void SNWGround::ReleaseBlock()
{
	CS.Unlock();
	return;
}

// Code書き込み
Void SNWGround::SetCode(Int32 x, Int32 y, Int32 z, SNMapchip::SNMapchipCode code)
{
	if (!IsProc())
	{
		SetCodeInfo[SetCodeInfoNum].X = (UInt16)x;
		SetCodeInfo[SetCodeInfoNum].Y = (UInt16)y;
		SetCodeInfo[SetCodeInfoNum].Z = (UInt16)z;
		SetCodeInfo[SetCodeInfoNum].Code = (UInt16)code;
		SetCodeInfoNum++;
	}

	return;
}

Void SNWGround::RunSetCode()
{
	if (!IsProc())
	{
		if (SetCodeInfoNum > 0)
		{
			State = SNWMeshStateSetCode;
			SNThread::Run();
		}
	}

	return;
}

// ファイルアクセス中？
Boolean SNWGround::IsProc()
{
	Boolean ret = false;

	// スレッド実行中またはIdle以外のとき
	if (GetRunStatus() || State != SNWMeshStateIdle)
	{
		ret = true;
	}

	return ret;
}

Void SNWGround::InitBlockList()
{
	// アドレス取得
	SNWGroundFileData* data_adr = (SNWGroundFileData*)BlockList.GetAddress();

	// タグ設定
	data_adr->Tag[0] = SNSystemConfig::WorldGroundFileTag[0];
	data_adr->Tag[1] = SNSystemConfig::WorldGroundFileTag[1];
	data_adr->Tag[2] = SNSystemConfig::WorldGroundFileTag[2];
	data_adr->Tag[3] = SNSystemConfig::WorldGroundFileTag[3];

	data_adr->BlockNum = 0;

	return;
}


Void SNWGround::UserMain()
{
	switch (State)
	{
	case SNWMeshStateRead:
		ReadMeshFile();
		break;

	case SNWMeshStateWrite:
		WriteMeshFile();
		break;
	case SNWMeshStateSetCode:
		SetCodeAsync();
		break;
	}

	State = SNWMeshStateIdle;

	return;
}

Void SNWGround::ReadMeshFile()
{
	SNFile file;
	SNWGroundFileData* data_adr;

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
	else
	{
		data_adr = (SNWGroundFileData*)BlockList.GetAddress();

		data_adr->BlockNum = 0;
	}

	return;
}

Void SNWGround::WriteMeshFile()
{
	SNFile file;
	SNWGroundFileData* data_adr = (SNWGroundFileData*)BlockList.GetAddress();
	UInt32 file_size = 16 + sizeof(SNWGroundBlockData) * data_adr->BlockNum;

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

Void SNWGround::SetCodeAsync()
{
	Int64 cnt;
	SNWGroundFileData* data_adr = (SNWGroundFileData*)BlockList.GetAddress();
	Int32 code_num = SetCodeInfoNum;
	Int32 code_index = 0;

	while (code_num > 0)
	{
		for (cnt = 0; cnt < data_adr->BlockNum; cnt++)
		{
			// 同一座標のデータあり
			if ((data_adr->Block[cnt].X) == (SetCodeInfo[code_index].X) &&
				(data_adr->Block[cnt].Y) == (SetCodeInfo[code_index].Y) &&
				(data_adr->Block[cnt].Z) == (SetCodeInfo[code_index].Z))
			{
				break;
			}
		}


		{
			SNAutoResource res(&CS);

			// 同一座標のブロックなし
			if (cnt >= data_adr->BlockNum)
			{
				// Blankは登録しない
				if (SetCodeInfo[code_index].Code != SNMapchip::SNMapchipBlank)
				{
					if (data_adr->BlockNum < SNGroundBlockDataSize - 1)
					{
						data_adr->Block[cnt] = SetCodeInfo[code_index];
						data_adr->BlockNum++;
					}
				}
			}

			// 同一座標のブロック発見
			else
			{
				// コードがBlank=削除のときは最終データを持ってくる
				if (SetCodeInfo[code_index].Code == SNMapchip::SNMapchipBlank)
				{
					data_adr->Block[cnt] = data_adr->Block[data_adr->BlockNum - 1];
					data_adr->BlockNum--;
				}
				else
				{
					// コードを上書きする
					data_adr->Block[cnt] = SetCodeInfo[code_index];
				}
			}
		}

		code_num--;
		code_index++;
	}

	SetCodeInfoNum = 0;

	return;
}


Void SNWGround::SetFileName(SNFile* file)
{
	SNMemory str;

	// パス設定
	file->SetFolderPath(SNSystemConfig::WorldMeshPath);

	// ファイル名設定
	str.Allocate(MAX_PATH * sizeof(Char));

	wsprintf((LPWSTR)str.GetAddress(), (LPWSTR)SNSystemConfig::WorldGroundFileName, MeshX, MeshY, MeshZ);

	file->SetFolderFileName((String)str.GetAddress());

	return;
}

Boolean SNWGround::IsValidFile(SNFile* file)
{
	Boolean ret = false;
	SNWGroundFileData* data;

	if (file->GetResult() == SNStorageResultNormal)
	{
		data = (SNWGroundFileData*)file->GetDataAddress();

		if (data != nullptr)
		{
			if ((data->Tag[0] == SNSystemConfig::WorldGroundFileTag[0]) &&
				(data->Tag[1] == SNSystemConfig::WorldGroundFileTag[1]) &&
				(data->Tag[2] == SNSystemConfig::WorldGroundFileTag[2]) &&
				(data->Tag[3] == SNSystemConfig::WorldGroundFileTag[3]))
			{
				ret = true;
			}
		}
	}

	return ret;
}

