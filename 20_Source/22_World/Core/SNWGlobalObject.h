#pragma once
#include "SNFrameworkInternal.h"
#include "SNThread.h"
#include "SNWObjectBase.h"
#include "SNFile.h"
#include "SNWNearbySpace.h"

// グローバルオブジェクト

// オブジェクトID
enum SNWGlobalObjectID
{
	SNWGlobalObjectPlayer,
	SNWGlobalObjectNum
};


struct SNWGlobalObjectFileData
{
	Char		Tag[4];
	SNWObjectInfo Object[SNWGlobalObjectNum];
};


class SNWGlobalObject : private SNThread
{
public:
	SNWGlobalObject();

	virtual ~SNWGlobalObject();

	// 初期化
	virtual Void Initialize();

	// 終了
	virtual Void Terminate();

	// Load
	virtual Void Load();

	// Save
	virtual Void Save();

	// ファイルアクセス中？
	virtual Boolean IsProc();

	// オブジェクト参照
	virtual SNWObjectBase* RefObject(SNWGlobalObjectID obj_id);

	virtual Void InitFileData(SNWGlobalObjectFileData* obj_file);

	virtual Void InitObjectBase();

	virtual Void CopyObjectInfoFromFile(SNWObjectInfo* obj_info_array);

	virtual Void CopyObjectInfoToFile(SNWObjectInfo* obj_info_array);

	// 更新
	virtual Void Update();

	// オブジェクト登録
	virtual Void RegisterNearbyObject(SNWNearbySpace* space);

private:
	virtual Void UserMain();

	virtual Void ReadGlobalObjectFile();
	
	virtual Void WriteGlobalObjectFile();

	virtual Void SetFileName(SNFile* file);

	virtual Boolean IsValidFile(SNFile* file);

	SNWMeshState State;
	SNWObjectBase ObjectArray[SNWGlobalObjectNum];
};
