#pragma once
#include "SNFrameworkInternal.h"
#include "SNBitmap.h"
#include "SNStore.h"

// ビットマップフォントクラス
class SNBitmapFont
{
public:
	// 初期化処理
	// 内部でファイルの非同期読み込みと完了待を行う
	// 失敗時はfalseを返す
	static Void Initialize();

	// 拡張機能初期化
	static Void InitializeExtraFunc();

	// 終了処理
	static Void Terminate();

	// システムテキスト描画
	static Void DrawSystemText(Int32 x, Int32 y, BMString str, UInt32 len, SNColor* color);

	// String→BMString
	static Void StringToBMString(String str, Int32 len, BMString bmstr, Int32 bmlen);

	// BMChar→Char変換
	static Char BMCharToChar(BMChar bmch);

	// BMString→String変換
	static Void BMStringToString(BMString bmstr, Int32 bmlen, String str, Int32 len);

	// ビットマップフォント座標取得
	static SNPoint GetBMCharPoint(BMChar bmch);

	// ビットマップフォントページ取得
	static UInt8 GetBMCharPage(BMChar bmch);
	
	// ビットマップフォントコード取得
	static UInt8 GetBMCharCode(BMChar bmch);

private:

};
