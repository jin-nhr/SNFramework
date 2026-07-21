#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsContext.h"
#include "SNBitmap.h"

// ビットマップフォントクラス
class SNBitmapFont
{
public:
	// 初期化処理
	// 内部でファイルの非同期読み込みと完了待を行う
	// 失敗時はfalseを返す
	static Void Initialize();

	// 終了処理
	static Void Terminate();

	// システムテキスト描画
	static Void DrawSystemText(SNGraphicsContext* dst_dc, Int32 x, Int32 y, BMString str, UInt32 len);

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

	// ビットマップフォントテーブル数
	static constexpr UInt32 BMCharTableNum = 256;

	// ビットマップフォント幅/高さ
	static constexpr UInt32 BMCharWidth = 16;
	static constexpr UInt32 BMCharHeight = 24;

	// ビットマップフォント縦横ブロック数
	static constexpr UInt32 BMCharBlockNumX = 16;
	static constexpr UInt32 BMCharBlockNumY = 16;

private:
};
