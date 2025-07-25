#pragma once
#include "../Include/SNFramework.h"


// DDBクラス

class SNDDB
{
public:
	// コンストラクタ
	SNDDB();

	// デストラクタ
	~SNDDB();

	// DDB生成
	Void CreateDDB(Int32 width, Int32 height);

	// DDB破棄
	Void DeleteDDB();

	// ハンドル取得
	Handle GetDDB();

	// 幅取得
	Int32 GetWidth();

	// 高さ取得
	Int32 GetHeight();

private:
	Handle DDBHandle;	// DDBハンドル
	Int32 Width;		// 幅
	Int32 Height;		// 高さ
};
