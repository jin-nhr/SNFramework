#include "SNWGlobalObject.h"
#include "SNSystemConfig.h"
#include "SNWObjectChip.h"
#include "SNWindowsAPI.h"



SNWGlobalObject::SNWGlobalObject()
{
	State = SNWMeshStateIdle;

	return;
}

SNWGlobalObject::~SNWGlobalObject()
{
	return;
}

// 初期化
Void SNWGlobalObject::Initialize()
{
	State = SNWMeshStateIdle;

	// オブジェクト初期化
	InitObjectBase();

	return;
}

// 終了
Void SNWGlobalObject::Terminate()
{
	// 処理中だったら終了を待つ
	SNThread::WaitForThreadEnd();

	return;
}

// Load
Void SNWGlobalObject::Load()
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
Void SNWGlobalObject::Save()
{
	if (!IsProc())
	{
		State = SNWMeshStateWrite;

		// スレッド実行
		SNThread::Run();
	}

	return;
}

SNWObjectBase* SNWGlobalObject::RefObject(SNWGlobalObjectID obj_id)
{
	return &ObjectArray[obj_id];
}

// ファイルアクセス中？
Boolean SNWGlobalObject::IsProc()
{
	Boolean ret = false;

	// スレッド実行中またはIdle以外のとき
	if (GetRunStatus() || State != SNWMeshStateIdle)
	{
		ret = true;
	}

	return ret;
}

Void SNWGlobalObject::InitFileData(SNWGlobalObjectFileData* obj_file)
{
	Int32 cnt;
	SNWObjectInfo* info;

	for (cnt = 0; cnt < 4; cnt++)
	{
		// タグ設定
		obj_file->Tag[cnt] = SNSystemConfig::WorldGlobalObjectFileTag[cnt];
	}

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		info = &obj_file->Object[cnt];
		
		info->Code = SNWObjectchip::WObjectCodeMaleOrg;
		info->AnimationStep = 0;
		info->AnimationCount = 0;
		info->Enable = false;
		info->Visible = false;
		info->Weight = false;
		info->State = SNWObjectStateIdle;
		info->FrontDir = SNWorldDirS;
		info->Pos.X = 0;
		info->Pos.Y = 0;
		info->Pos.Y = 0;
		info->Acceleration.X = 0;
		info->Acceleration.Y = 0;
		info->Acceleration.Z = 0;
		info->Speed.X = 0;
		info->Speed.Y = 0;
		info->Speed.Z = 0;
	}

	return;
}

Void SNWGlobalObject::InitObjectBase()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		ObjectArray[cnt].Initialize();
	}

	return;
}

// ファイルデータからクラスへのデータコピー
Void SNWGlobalObject::CopyObjectInfoFromFile(SNWObjectInfo* obj_info_array)
{
	Int32 cnt;

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		*(ObjectArray[cnt].RefInfo()) = obj_info_array[cnt];
	}
	return;
}

// クラスからファイルへのデータコピー
Void SNWGlobalObject::CopyObjectInfoToFile(SNWObjectInfo* obj_info_array)
{
	Int32 cnt;

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		obj_info_array[cnt] = *(ObjectArray[cnt].RefInfo());
	}
	return;

	return;
}


Void SNWGlobalObject::Update()
{
	Int32 cnt;

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		ObjectArray[cnt].Update();
	}

	return;
}

Void SNWGlobalObject::RegisterNearbyObject(SNWNearbySpace* space)
{
	Int32 cnt;
	SNWObjectInfo* obj_info;

	for (cnt = 0; cnt < SNWGlobalObjectNum; cnt++)
	{
		obj_info = ObjectArray[cnt].RefInfo();

		// 有効オブジェクト
		if (obj_info->Enable)
		{
			// 周辺空間内のオブジェクトかチェック
			if (space->CollisionCellVSSpace(&obj_info->Pos))
			{
				// オブジェクト登録
				space->RegisterGObjectData(&ObjectArray[cnt]);
			}
		}
	}

	return;
}


Void SNWGlobalObject::UserMain()
{
	switch (State)
	{
	case SNWMeshStateRead:
		ReadGlobalObjectFile();
		break;

	case SNWMeshStateWrite:
		WriteGlobalObjectFile();
		break;
	}

	State = SNWMeshStateIdle;

	return;
}

Void SNWGlobalObject::ReadGlobalObjectFile()
{
	SNFile file;
	SNWGlobalObjectFileData* io_data;

	// ファイル情報設定
	SetFileName(&file);

	// ファイル読み込み
	file.Read();

	// 完了待ち
	file.WaitForOperationComplete();

	// ファイルチェック
	if (IsValidFile(&file))
	{
		io_data = (SNWGlobalObjectFileData*)file.GetDataAddress();

		CopyObjectInfoFromFile(io_data->Object);
	}
	else
	{
		InitObjectBase();
	}

	return;
}

Void SNWGlobalObject::WriteGlobalObjectFile()
{
	SNFile file;
	SNWGlobalObjectFileData* io_data;
	UInt32 file_size = sizeof(SNWGlobalObjectFileData);

	// ファイル情報設定
	SetFileName(&file);

	// メモリ確保
	file.GetMemoryObject()->Allocate(file_size);

	io_data = (SNWGlobalObjectFileData*)file.GetMemoryObject()->GetAddress();

	// ファイルデータ初期化
	InitFileData(io_data);

	// オブジェクト情報コピー
	CopyObjectInfoToFile(io_data->Object);

	// ファイル書き込み
	file.Write();

	// 完了待ち
	file.WaitForOperationComplete();

	return;
}

Void SNWGlobalObject::SetFileName(SNFile* file)
{
	SNMemory str;

	// パス設定
	file->SetFolderPath(SNSystemConfig::WorldPath);

	// ファイル名設定
	str.Allocate(MAX_PATH * sizeof(Char));

	wsprintf((LPWSTR)str.GetAddress(), (LPWSTR)SNSystemConfig::WorldGlobalObjectFileName);

	file->SetFolderFileName((String)str.GetAddress());

	return;
}

Boolean SNWGlobalObject::IsValidFile(SNFile* file)
{
	Boolean ret = false;
	SNWGlobalObjectFileData* data;

	if (file->GetResult() == SNStorageResultNormal)
	{
		data = (SNWGlobalObjectFileData*)file->GetDataAddress();

		if (data != nullptr)
		{
			if ((data->Tag[0] == SNSystemConfig::WorldGlobalObjectFileTag[0]) &&
				(data->Tag[1] == SNSystemConfig::WorldGlobalObjectFileTag[1]) &&
				(data->Tag[2] == SNSystemConfig::WorldGlobalObjectFileTag[2]) &&
				(data->Tag[3] == SNSystemConfig::WorldGlobalObjectFileTag[3]))
			{
				ret = true;
			}
		}
	}

	return ret;
}

