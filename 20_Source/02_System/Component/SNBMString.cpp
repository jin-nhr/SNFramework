#include "SNBMString.h"
#include "SNBitmapFont.h"
#include "SNWindowsAPI.h"

// ビットマップフォント文字列クラス

// コンストラクタ
SNBMString::SNBMString()
{
	// 空白文字をセットしておく
	StringBuffer.Allocate(sizeof(BMChar) * 1);
	((BMString)StringBuffer.GetAddress())[0] = 0;
	Length = 1;

	return;
}

// コンストラクタ(文字列設定)
SNBMString::SNBMString(String str)
{
	// 文字列設定
	SetString(str);

	return;
}

// コンストラクタ(文字列設定)
SNBMString::SNBMString(BMString str, Int32 length)
{
	// 文字列設定
	SetString(str, length);

	return;
}

// デストラクタ
SNBMString::~SNBMString()
{
	StringBuffer.Clear();
	return;
}

// 文字列取得
BMString SNBMString::GetString()
{
	return (BMString)StringBuffer.GetAddress();
}

// 文字列長取得
Int32 SNBMString::GetLength()
{
	return Length;
}

// メモリ確保長
Int32 SNBMString::GetAllocLength()
{
	return (StringBuffer.GetAllocSize() / sizeof(BMChar));
}

// 文字列設定
Void SNBMString::SetString(String str)
{
	Int32 str_len = lstrlen(str);
	UInt32 alloc_size = sizeof(BMChar) * (str_len);
	BMString bmstr;

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	bmstr = (BMString)StringBuffer.GetAddress();

	// StringからBMStringへ変換
	SNBitmapFont::StringToBMString(str, str_len, bmstr, str_len);

	// 文字列長のセット
	Length = str_len;

	return;
}

// 文字列設定
Void SNBMString::SetString(BMString str, Int32 length)
{
	BMString copy_str = new BMChar[length];
	UInt32 alloc_size = sizeof(BMChar) * (length);

	// いったんstrを内部変数にコピー
	CopyMemory(copy_str, str, sizeof(BMChar) * length);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	CopyMemory(StringBuffer.GetAddress(), copy_str, sizeof(BMChar) * length);

	// 文字列長のセット
	Length = length;

	// 内部変数解放
	delete[] copy_str;

	return;
}

// 文字列設定(連結)
Void SNBMString::SetString(BMString str1, Int32 length1, BMString str2, Int32 length2)
{
	Int32 str_len = length1 + length2;
	BMString copy_str = new BMChar[str_len];
	UInt32 alloc_size = sizeof(BMChar) * (str_len);

	// いったんstrを内部変数にコピー
	CopyMemory(&copy_str[0], str1, sizeof(BMChar) * length1);
	CopyMemory(&copy_str[length1], str2, sizeof(BMChar) * length2);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	CopyMemory(StringBuffer.GetAddress(), copy_str, sizeof(BMChar) * str_len);

	// 文字列長のセット
	Length = str_len;

	// 内部変数解放
	delete[] copy_str;

	return;
}

// 文字列追加
Void SNBMString::AddString(BMString str, Int32 length)
{
	Int32 str_len = Length + length;
	BMString copy_str = new BMChar[str_len];
	UInt32 alloc_size = sizeof(BMChar) * (str_len);

	// いったんstrを内部変数にコピー
	CopyMemory(&copy_str[0], StringBuffer.GetAddress(), sizeof(BMChar) * Length);
	CopyMemory(&copy_str[Length], str, sizeof(BMChar) * length);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	CopyMemory(StringBuffer.GetAddress(), copy_str, sizeof(BMChar) * str_len);

	// 文字列長のセット
	Length = str_len;

	// 内部変数解放
	delete[] copy_str;
}

// フォーマット文字設定
Void SNBMString::Print(String format, Int64 value)
{
	// フォーマット後の長さを取得
	Int32 need_len = _scwprintf(format, value);

	// ワークバッファ確保
	String cnv_str = new Char[need_len + 1];

	// フォーマットして書き込み
	StringCchPrintfW(cnv_str, need_len + 1, format, value);

	// メモリ確保
	StringBuffer.Allocate(sizeof(BMChar) * need_len);

	// StringからBMStringへ変換
	SNBitmapFont::StringToBMString(cnv_str, need_len, (BMString)StringBuffer.GetAddress(), need_len);

	// 長さ更新（終端除く）
	Length = need_len;

	// 内部変数解放
	delete[] cnv_str;

	return;
}

// クリア
Void SNBMString::Clear()
{
	// メモリ解放し空白文字のみセット
	StringBuffer.Allocate(sizeof(BMChar) * 1);
	((BMString)StringBuffer.GetAddress())[0] = 0;
	Length = 1;
	return;
}

// メモリ事前確保(内容はクリアされる)
Void SNBMString::PreAllocate(UInt32 length)
{
	// 指定した長さのメモリを確保し空白文字のみをセット
	StringBuffer.Allocate(sizeof(BMChar) * length);
	((BMString)StringBuffer.GetAddress())[0] = 0;
	Length = 1;
	return;
}

// 文字置き換え (char1→char2に置き換え)
Void SNBMString::ReplaceCharacter(BMChar char1, BMChar char2)
{
	Int32 cnt;
	BMString str = GetString();

	// 文字列走査し、char1があったらchar2に置き換える
	for (cnt = 0; cnt < Length; cnt++)
	{
		if (str[cnt] == char1)
		{
			str[cnt] = char2;
		}
	}

	return;
}

// 文字列のイメージサイズ
SNSize SNBMString::GetStringImageSize()
{
	SNSize size;

	size.Width = Length * SNBitmapFont::BMCharWidth;
	size.Height = SNBitmapFont::BMCharHeight;

	return size;
}
