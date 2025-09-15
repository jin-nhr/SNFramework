#pragma once
#include "../Include/SNFramework.h"
#include "SNMemory.h"

// 文字列クラス
class SNString
{
public:
	// コンストラクタ
	SNString();

	// コンストラクタ
	SNString(String str);

	// デストラクタ
	~SNString();

	// 文字列取得
	String GetString();

	// 文字列長取得
	Int32 GetLength();

	// メモリ確保長
	Int32 GetAllocLength();

	// 文字列設定
	Void SetString(String str);

	// 文字列設定(連結)
	Void SetString(String str1, String str2);

	// 文字列追加
	Void AddString(String str);

	// フォーマット文字設定
	Void Print(String format, Int64 value);

	// クリア
	Void Clear();

	// メモリ事前確保(内容はクリアされる)
	Void PreAllocate(UInt32 length);

	// 文字置き換え (char1→char2に置き換え)
	Void ReplaceCharacter(Char char1, Char char2);

private:
	SNMemory StringBuffer;	// 文字列バッファ
	Int32 Length;			// 文字列長
};
