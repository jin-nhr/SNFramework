#include "SNString.h"
#include "../System/SNWindowsAPI.h"

// 文字列クラス


// コンストラクタ
SNString::SNString()
{
	// 終端文字をセットしておく
	StringBuffer.Allocate(sizeof(Char) * 1);
	((String)StringBuffer.GetAddress())[0] = L'\0';
	Length = 0;

	return;
}

// コンストラクタ
SNString::SNString(String str)
{
	// デフォルトコンストラクタ実行
	SNString();

	// 文字列設定
	SetString(str);

	return;
}

// デストラクタ
SNString::~SNString()
{
	StringBuffer.Clear();
	return;
}

// 文字列取得
String SNString::GetString()
{
	return (String)StringBuffer.GetAddress();
}

// 文字列長取得
Int32 SNString::GetLength()
{
	return Length;
}

// メモリ確保長取得
Int32 SNString::GetAllocLength()
{
	return (StringBuffer.GetAllocSize() / sizeof(Char));
}

// 文字列設定
Void SNString::SetString(String str)
{
	Int32 str_len = lstrlen(str);
	String copy_str = new Char[str_len + 1];
	UInt32 alloc_size = sizeof(Char) * (str_len + 1);

	// いったんstrを内部変数にコピー
	swprintf_s(copy_str, str_len + 1, L"%s", str);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	swprintf_s((String)StringBuffer.GetAddress(), str_len + 1, L"%s", copy_str);

	// 文字列長のセット
	Length = str_len;

	// 内部変数解放
	delete[] copy_str;

	return;
}

// 文字列設定(連結)
Void SNString::SetString(String str1, String str2)
{
	Int32 str_len = lstrlen(str1) + lstrlen(str2);
	String copy_str = new Char[str_len + 1];
	UInt32 alloc_size = sizeof(Char) * (str_len + 1);

	// いったんstrを内部変数にコピー
	swprintf_s(copy_str, str_len + 1, L"%s%s", str1, str2);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	swprintf_s((String)StringBuffer.GetAddress(), str_len + 1, L"%s", copy_str);

	// 文字列長のセット
	Length = str_len;

	// 内部変数解放
	delete[] copy_str;

	return;
}

// 文字列追加
Void SNString::AddString(String str)
{
	Int32 str_len = lstrlen((String)StringBuffer.GetAddress()) + lstrlen(str);
	String copy_str = new Char[str_len + 1];
	UInt32 alloc_size = sizeof(Char) * (str_len + 1);

	// いったんstrを内部変数にコピー
	swprintf_s(copy_str, str_len + 1, L"%s%s", (String)StringBuffer.GetAddress(), str);

	// メモリ再確保
	StringBuffer.Allocate(alloc_size);

	// 文字列バッファにセット
	swprintf_s((String)StringBuffer.GetAddress(), str_len + 1, L"%s", copy_str);

	// 文字列長のセット
	Length = str_len;

	// 内部変数解放
	delete[] copy_str;

	return;
}

// フォーマット文字設定
Void SNString::Print(String format, Int64 value)
{
	// フォーマット後の長さを取得
	Int32 need_len = _scwprintf(format, value) + 1;

	// メモリ確保
	StringBuffer.Allocate(sizeof(Char) * need_len);

	// フォーマットして書き込み
	StringCchPrintfW((String)StringBuffer.GetAddress(), need_len, format, value);

	// 長さ更新（終端除く）
	Length = need_len - 1;

	return;
}

// クリア
Void SNString::Clear()
{
	// メモリ解放し終端文字のみセット
	StringBuffer.Allocate(sizeof(Char) * 1);
	((String)StringBuffer.GetAddress())[0] = L'\0';
	Length = 0;
	return;
}

// メモリ事前確保(内容はクリアされる)
Void SNString::PreAllocate(UInt32 length)
{
	// 指定した長さのメモリを確保し終端文字のみをセット
	StringBuffer.Allocate(sizeof(Char) * length);
	((String)StringBuffer.GetAddress())[0] = L'\0';
	Length = 0;
	return;
}

// 文字置き換え (char1→char2に置き換え)
Void SNString::ReplaceCharacter(Char char1, Char char2)
{
	Int32 cnt;
	String str = GetString();

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
