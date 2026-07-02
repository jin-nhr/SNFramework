#pragma once
#include "SNFrameworkInternal.h"
#include "SNMemory.h"

// 文字列クラス
// 内部バッファに文字列を格納し各種処理を行う
// 一度確保したバッファが足りる限りは再確保の動作はしない
// メモリリークを避けたい場合は必要に応じてクリアを実行すること
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
	// 文字列長と異なる点に注意
	Int32 GetAllocLength();

	// 文字列設定
	// 文字列を内部バッファに格納する
	Void SetString(String str);

	// 文字列設定(連結)
	// str1とstr2を結合した文字列を内部バッファに格納する
	// str1, str2として自身の保持している文字列の指定も可
	Void SetString(String str1, String str2);

	// 文字列追加
	// 内部バッファの文字列にstrを付け加える
	Void AddString(String str);

	// フォーマット文字設定
	// formatに指定したフォーマットの文字列を内部バッファに格納する
	// 可変の整数値を1つ指定可能
	Void Print(String format, Int64 value);

	// クリア
	// 内部バッファの解放、文字列のクリア
	Void Clear();

	// メモリ事前確保(内容はクリアされる)
	// あらかじめ格納する文字列の長さが分かっている場合は事前に実行することで
	// メモリ確保のオーバーヘッドを減らすことが出来る
	Void PreAllocate(UInt32 length);

	// 文字置き換え (char1→char2に置き換え)
	Void ReplaceCharacter(Char char1, Char char2);

private:
	SNMemory StringBuffer;	// 文字列バッファ
	Int32 Length;			// 文字列長
};
