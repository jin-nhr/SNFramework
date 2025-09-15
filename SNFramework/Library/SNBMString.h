#pragma once
#include "../Include/SNFramework.h"
#include "SNMemory.h"


// ビットマップフォントテーブル
struct SNBMCharTable
{
	Char	Character;	// UTF16文字コード
	UInt8	Reserve[2];	// リザーブ
	SNPoint	Point;		// ビットマップフォント座標
};

// ビットマップフォント文字列クラス
class SNBMString
{
public:
	// スタティックメンバ

	// ビットマップフォントテーブル数
	static constexpr UInt32 BMCharTableNum = 256;

	// ビットマップフォントテーブル
	static const SNBMCharTable BMCharTable[BMCharTableNum];

	// Char→BMChar変換
	static BMChar CharToBMChar(Char ch);

	// String→BMString
	static Void StringToBMString(String str, Int32 len, BMString bmstr, Int32 bmlen);

	// BMChar→Char変換
	static Char BMCharToChar(BMChar bmch);

	// BMString→String変換
	static Void BMStringToString(BMString bmstr, Int32 bmlen, String str, Int32 len);

	// ビットマップフォント座標取得
	static SNPoint GetBMCharPoint(BMChar bmch);

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

private:
	SNMemory StringBuffer;	// 文字列バッファ
	Int32 Length;			// 文字列長
};
