#pragma once
#include "SNFrameworkInternal.h"
#include "SNString.h"

// ミューテックスクラス
// 同名ミューテックスはOS上で一つのみ生成される特性を利用し
// アプリケーションの多重起動判定などに用いる
class SNMutex
{
public:
	// コンストラクタ
	SNMutex();

	// デストラクタ
	~SNMutex();

	// ミューテックス生成
	// パラメータ：最大256文字の文字列を設定
	// 同名ミューテックスが既に存在する場合はハンドルのみ取得される
	Void Create(String mutex_name);

	// ミューテックス破棄
	Void Delete();

	// 自身が最初のミューテックスか判定する
	// リターン：最初のミューテックスならtrueを返す
	Boolean IsFirst();

private:
	SNString MutexName;			// ミューテックス名
	Handle   MutexHandle;		// ミューテックスハンドル
	Boolean  FirstMutexFlag;	// 最初のミューテックスかを示すフラグ
};

