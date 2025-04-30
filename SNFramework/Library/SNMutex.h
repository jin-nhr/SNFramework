#pragma once
#include "../Include/SNFramework.h"
#include "SNMemory.h"

// ミューテックスクラス
class SNMutex
{
public:
	// コンストラクタ
	SNMutex();

	// デストラクタ
	~SNMutex();

	// ミューテックス生成
	// パラメータ：最大256文字の文字列を設定
	Void Create(String mutex_name);

	// ミューテックス破棄
	Void Delete();

	// 自身が最初のミューテックスか判定する
	// リターン：最初のミューテックスならtrueを返す
	Boolean IsFirst();

private:
	SNMemory MutexName;			// ミューテックス名
	Handle   MutexHandle;		// ミューテックスハンドル
	Boolean  FirstMutexFlag;	// 最初のミューテックスかを示すフラグ
};

