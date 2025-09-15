#include "SNMutex.h"
#include "../System/SNWindowsAPI.h"


// コンストラクタ
SNMutex::SNMutex()
{
	MutexHandle = nullptr;
	FirstMutexFlag = false;

	return;
}

// デストラクタ
SNMutex::~SNMutex()
{
	// ミューテックス生成済みの場合
	if (MutexHandle != nullptr)
	{
		CloseHandle((HANDLE)MutexHandle);
		FirstMutexFlag = false;
	}

	return;
}

// ミューテックス生成
// パラメータ：最大256文字の文字列を設定
Void SNMutex::Create(String mutex_name)
{
	// 名前保持領域の準備
	MutexName.SetString(mutex_name);

	// '\'は使用不可のため'/'に置き換える
	MutexName.ReplaceCharacter(L'\\', L'/');

	// Mutex生成
	MutexHandle = (void*)CreateMutex(NULL, TRUE, MutexName.GetString());

	// 生成成功し、システム上存在していなければ最初のミューテックスと判定する
	if (MutexHandle != nullptr)
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			FirstMutexFlag = true;
		}
	}

	return;
}

// ミューテックス破棄
Void SNMutex::Delete()
{
	// ミューテックス生成済みの場合
	if (MutexHandle != nullptr)
	{
		CloseHandle((HANDLE)MutexHandle);
		FirstMutexFlag = false;
	}

	return;
}

// 自身が最初のミューテックスか判定する
// リターン：最初のミューテックスならTrueを返す
Boolean SNMutex::IsFirst()
{
	return FirstMutexFlag;
}

