#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"


// ビットマップフォント文字列クラス
// ビットマップフォント用の文字コード列を管理する
// 通常の文字列と相互変換が可能でありSNStringと同等の機能を有する
// ビットマップフォント用コードは終端文字を持たない点に注意
class SNBMString
{
public:
	// コンストラクタ
	SNBMString();

	// コンストラクタ(文字列設定)
	SNBMString(String str);

	// コンストラクタ(文字列設定)
	SNBMString(BMString str, Int32 length);

	// デストラクタ
	~SNBMString();

	// 文字列取得
	BMString GetString();

	// 文字列長取得
	Int32 GetLength();

	// メモリ確保長
	Int32 GetAllocLength();

	// 文字列設定
	Void SetString(String str);

	// 文字列設定
	Void SetString(BMString str, Int32 length);

	// 文字列設定(連結)
	Void SetString(BMString str1, Int32 length1, BMString str2, Int32 length2);

	// 文字列追加
	Void AddString(BMString str, Int32 length);

	// フォーマット文字設定
	Void Print(String format, Int64 value);

	// クリア
	Void Clear();

	// メモリ事前確保(内容はクリアされる)
	Void PreAllocate(UInt32 length);

	// 文字置き換え (char1→char2に置き換え)
	Void ReplaceCharacter(BMChar char1, BMChar char2);

	// 文字列のイメージサイズ
	SNSize GetStringImageSize();

private:
	SNMemory StringBuffer;	// 文字列バッファ
	Int32 Length;			// 文字列長
};
