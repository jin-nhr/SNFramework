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
		MutexName.Free();
		FirstMutexFlag = false;
	}

	return;
}

// ミューテックス生成
// パラメータ：最大256文字の文字列を設定
Void SNMutex::Create(String mutex_name)
{
	Int cnt;
	Int len;
	UInt32 alloc_size;
	String save_mutex_name;
	
	// パラメータ文字列の長さを取得
	len = lstrlen(mutex_name);
	if (len > MAX_PATH)
	{
		// MAX_PATHまでに切る
		len = MAX_PATH;
	}

	// 名前保持領域の準備
	alloc_size = sizeof(Char) * (len + 1);
	MutexName.Allocate(alloc_size);
	MutexName.Clear();
	MutexName.Copy((Void*)mutex_name, alloc_size);
	save_mutex_name = (String)MutexName.GetAddress();

	// '\'は使用不可のため'/'に置き換える
	for (cnt = 0; cnt < len; cnt++)
	{
		if (save_mutex_name[cnt] == L'\\')
		{
			save_mutex_name[cnt] = L'/';
		}
	}

	// ミューテックス生成
	MutexHandle = (void*)CreateMutex(NULL, TRUE, save_mutex_name);

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
		MutexName.Free();
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

