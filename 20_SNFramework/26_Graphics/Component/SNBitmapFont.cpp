#include "SNBitmapFont.h"
#include "SNImageFile.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"
#include "SNConfig.h"
#include "SNGraphics.h"
#include "SNAutoResource.h"
#include "SNSystemBrush.h"

// ビットマップフォントクラス

// ビットマップフォントテーブル
const SNBMCharTable SNBitmapFont::BMCharTable[] =
{
	{L'　', 0, 0, 0, 0},
	{ L'！', 0, 0, 16, 0 },
	{ L'”', 0, 0, 32, 0 },
	{ L'＃', 0, 0, 48, 0 },
	{ L'＄', 0, 0, 64, 0 },
	{ L'％', 0, 0, 80, 0 },
	{ L'＆', 0, 0, 96, 0 },
	{ L'（', 0, 0, 112, 0 },
	{ L'）', 0, 0, 128, 0 },
	{ L'＝', 0, 0, 144, 0 },
	{ L'－', 0, 0, 160, 0 },
	{ L'￥', 0, 0, 176, 0 },
	{ L'＋', 0, 0, 192, 0 },
	{ L'＊', 0, 0, 208, 0 },
	{ L'？', 0, 0, 224, 0 },
	{ L'／', 0, 0, 240, 0 },
	{ L'０', 0, 0, 0, 24 },
	{ L'１', 0, 0, 16, 24 },
	{ L'２', 0, 0, 32, 24 },
	{ L'３', 0, 0, 48, 24 },
	{ L'４', 0, 0, 64, 24 },
	{ L'５', 0, 0, 80, 24 },
	{ L'６', 0, 0, 96, 24 },
	{ L'７', 0, 0, 112, 24 },
	{ L'８', 0, 0, 128, 24 },
	{ L'９', 0, 0, 144, 24 },
	{ L'ａ', 0, 0, 160, 24 },
	{ L'ｂ', 0, 0, 176, 24 },
	{ L'ｃ', 0, 0, 192, 24 },
	{ L'ｄ', 0, 0, 208, 24 },
	{ L'ｅ', 0, 0, 224, 24 },
	{ L'ｆ', 0, 0, 240, 24 },
	{ L'ｇ', 0, 0, 0, 48 },
	{ L'ｈ', 0, 0, 16, 48 },
	{ L'ｉ', 0, 0, 32, 48 },
	{ L'ｊ', 0, 0, 48, 48 },
	{ L'ｋ', 0, 0, 64, 48 },
	{ L'ｌ', 0, 0, 80, 48 },
	{ L'ｍ', 0, 0, 96, 48 },
	{ L'ｎ', 0, 0, 112, 48 },
	{ L'ｏ', 0, 0, 128, 48 },
	{ L'ｐ', 0, 0, 144, 48 },
	{ L'ｑ', 0, 0, 160, 48 },
	{ L'ｒ', 0, 0, 176, 48 },
	{ L'ｓ', 0, 0, 192, 48 },
	{ L'ｔ', 0, 0, 208, 48 },
	{ L'ｕ', 0, 0, 224, 48 },
	{ L'ｖ', 0, 0, 240, 48 },
	{ L'ｗ', 0, 0, 0, 72 },
	{ L'ｘ', 0, 0, 16, 72 },
	{ L'ｙ', 0, 0, 32, 72 },
	{ L'ｚ', 0, 0, 48, 72 },
	{ L'Ａ', 0, 0, 64, 72 },
	{ L'Ｂ', 0, 0, 80, 72 },
	{ L'Ｃ', 0, 0, 96, 72 },
	{ L'Ｄ', 0, 0, 112, 72 },
	{ L'Ｅ', 0, 0, 128, 72 },
	{ L'Ｆ', 0, 0, 144, 72 },
	{ L'Ｇ', 0, 0, 160, 72 },
	{ L'Ｈ', 0, 0, 176, 72 },
	{ L'Ｉ', 0, 0, 192, 72 },
	{ L'Ｊ', 0, 0, 208, 72 },
	{ L'Ｋ', 0, 0, 224, 72 },
	{ L'Ｌ', 0, 0, 240, 72 },
	{ L'Ｍ', 0, 0, 0, 96 },
	{ L'Ｎ', 0, 0, 16, 96 },
	{ L'Ｏ', 0, 0, 32, 96 },
	{ L'Ｐ', 0, 0, 48, 96 },
	{ L'Ｑ', 0, 0, 64, 96 },
	{ L'Ｒ', 0, 0, 80, 96 },
	{ L'Ｓ', 0, 0, 96, 96 },
	{ L'Ｔ', 0, 0, 112, 96 },
	{ L'Ｕ', 0, 0, 128, 96 },
	{ L'Ｖ', 0, 0, 144, 96 },
	{ L'Ｗ', 0, 0, 160, 96 },
	{ L'Ｘ', 0, 0, 176, 96 },
	{ L'Ｙ', 0, 0, 192, 96 },
	{ L'Ｚ', 0, 0, 208, 96 },
	{ L'［', 0, 0, 224, 96 },
	{ L'］', 0, 0, 240, 96 },
	{ L'｛', 0, 0, 0, 120 },
	{ L'｝', 0, 0, 16, 120 },
	{ L'「', 0, 0, 32, 120 },
	{ L'」', 0, 0, 48, 120 },
	{ L'＜', 0, 0, 64, 120 },
	{ L'＞', 0, 0, 80, 120 },
	{ L'＠', 0, 0, 96, 120 },
	{ L'※', 0, 0, 112, 120 },
	{ L'ー', 0, 0, 128, 120 },
	{ L'、', 0, 0, 144, 120 },
	{ L'。', 0, 0, 160, 120 },
	{ L'，', 0, 0, 176, 120 },
	{ L'．', 0, 0, 192, 120 },
	{ L'・', 0, 0, 208, 120 },
	{ L'’', 0, 0, 224, 120 },
	{ L'ヴ', 0, 0, 240, 120 },
	{ L'あ', 0, 0, 0, 144 },
	{ L'い', 0, 0, 16, 144 },
	{ L'う', 0, 0, 32, 144 },
	{ L'え', 0, 0, 48, 144 },
	{ L'お', 0, 0, 64, 144 },
	{ L'か', 0, 0, 80, 144 },
	{ L'き', 0, 0, 96, 144 },
	{ L'く', 0, 0, 112, 144 },
	{ L'け', 0, 0, 128, 144 },
	{ L'こ', 0, 0, 144, 144 },
	{ L'さ', 0, 0, 160, 144 },
	{ L'し', 0, 0, 176, 144 },
	{ L'す', 0, 0, 192, 144 },
	{ L'せ', 0, 0, 208, 144 },
	{ L'そ', 0, 0, 224, 144 },
	{ L'た', 0, 0, 240, 144 },
	{ L'ち', 0, 0, 0, 168 },
	{ L'つ', 0, 0, 16, 168 },
	{ L'て', 0, 0, 32, 168 },
	{ L'と', 0, 0, 48, 168 },
	{ L'な', 0, 0, 64, 168 },
	{ L'に', 0, 0, 80, 168 },
	{ L'ぬ', 0, 0, 96, 168 },
	{ L'ね', 0, 0, 112, 168 },
	{ L'の', 0, 0, 128, 168 },
	{ L'は', 0, 0, 144, 168 },
	{ L'ひ', 0, 0, 160, 168 },
	{ L'ふ', 0, 0, 176, 168 },
	{ L'へ', 0, 0, 192, 168 },
	{ L'ほ', 0, 0, 208, 168 },
	{ L'ま', 0, 0, 224, 168 },
	{ L'み', 0, 0, 240, 168 },
	{ L'む', 0, 0, 0, 192 },
	{ L'め', 0, 0, 16, 192 },
	{ L'も', 0, 0, 32, 192 },
	{ L'や', 0, 0, 48, 192 },
	{ L'ゆ', 0, 0, 64, 192 },
	{ L'よ', 0, 0, 80, 192 },
	{ L'ら', 0, 0, 96, 192 },
	{ L'り', 0, 0, 112, 192 },
	{ L'る', 0, 0, 128, 192 },
	{ L'れ', 0, 0, 144, 192 },
	{ L'ろ', 0, 0, 160, 192 },
	{ L'わ', 0, 0, 176, 192 },
	{ L'を', 0, 0, 192, 192 },
	{ L'ん', 0, 0, 208, 192 },
	{ L'が', 0, 0, 224, 192 },
	{ L'ぎ', 0, 0, 240, 192 },
	{ L'ぐ', 0, 0, 0, 216 },
	{ L'げ', 0, 0, 16, 216 },
	{ L'ご', 0, 0, 32, 216 },
	{ L'ざ', 0, 0, 48, 216 },
	{ L'じ', 0, 0, 64, 216 },
	{ L'ず', 0, 0, 80, 216 },
	{ L'ぜ', 0, 0, 96, 216 },
	{ L'ぞ', 0, 0, 112, 216 },
	{ L'だ', 0, 0, 128, 216 },
	{ L'ぢ', 0, 0, 144, 216 },
	{ L'づ', 0, 0, 160, 216 },
	{ L'で', 0, 0, 176, 216 },
	{ L'ど', 0, 0, 192, 216 },
	{ L'ば', 0, 0, 208, 216 },
	{ L'び', 0, 0, 224, 216 },
	{ L'ぶ', 0, 0, 240, 216 },
	{ L'べ', 0, 0, 0, 240 },
	{ L'ぼ', 0, 0, 16, 240 },
	{ L'ぱ', 0, 0, 32, 240 },
	{ L'ぴ', 0, 0, 48, 240 },
	{ L'ぷ', 0, 0, 64, 240 },
	{ L'ぺ', 0, 0, 80, 240 },
	{ L'ぽ', 0, 0, 96, 240 },
	{ L'ぁ', 0, 0, 112, 240 },
	{ L'ぃ', 0, 0, 128, 240 },
	{ L'ぅ', 0, 0, 144, 240 },
	{ L'ぇ', 0, 0, 160, 240 },
	{ L'ぉ', 0, 0, 176, 240 },
	{ L'っ', 0, 0, 192, 240 },
	{ L'ゃ', 0, 0, 208, 240 },
	{ L'ゅ', 0, 0, 224, 240 },
	{ L'ょ', 0, 0, 240, 240 },
	{ L'ア', 0, 0, 0, 264 },
	{ L'イ', 0, 0, 16, 264 },
	{ L'ウ', 0, 0, 32, 264 },
	{ L'エ', 0, 0, 48, 264 },
	{ L'オ', 0, 0, 64, 264 },
	{ L'カ', 0, 0, 80, 264 },
	{ L'キ', 0, 0, 96, 264 },
	{ L'ク', 0, 0, 112, 264 },
	{ L'ケ', 0, 0, 128, 264 },
	{ L'コ', 0, 0, 144, 264 },
	{ L'サ', 0, 0, 160, 264 },
	{ L'シ', 0, 0, 176, 264 },
	{ L'ス', 0, 0, 192, 264 },
	{ L'セ', 0, 0, 208, 264 },
	{ L'ソ', 0, 0, 224, 264 },
	{ L'タ', 0, 0, 240, 264 },
	{ L'チ', 0, 0, 0, 288 },
	{ L'ツ', 0, 0, 16, 288 },
	{ L'テ', 0, 0, 32, 288 },
	{ L'ト', 0, 0, 48, 288 },
	{ L'ナ', 0, 0, 64, 288 },
	{ L'ニ', 0, 0, 80, 288 },
	{ L'ヌ', 0, 0, 96, 288 },
	{ L'ネ', 0, 0, 112, 288 },
	{ L'ノ', 0, 0, 128, 288 },
	{ L'ハ', 0, 0, 144, 288 },
	{ L'ヒ', 0, 0, 160, 288 },
	{ L'フ', 0, 0, 176, 288 },
	{ L'ヘ', 0, 0, 192, 288 },
	{ L'ホ', 0, 0, 208, 288 },
	{ L'マ', 0, 0, 224, 288 },
	{ L'ミ', 0, 0, 240, 288 },
	{ L'ム', 0, 0, 0, 312 },
	{ L'メ', 0, 0, 16, 312 },
	{ L'モ', 0, 0, 32, 312 },
	{ L'ヤ', 0, 0, 48, 312 },
	{ L'ユ', 0, 0, 64, 312 },
	{ L'ヨ', 0, 0, 80, 312 },
	{ L'ラ', 0, 0, 96, 312 },
	{ L'リ', 0, 0, 112, 312 },
	{ L'ル', 0, 0, 128, 312 },
	{ L'レ', 0, 0, 144, 312 },
	{ L'ロ', 0, 0, 160, 312 },
	{ L'ワ', 0, 0, 176, 312 },
	{ L'ヲ', 0, 0, 192, 312 },
	{ L'ン', 0, 0, 208, 312 },
	{ L'ガ', 0, 0, 224, 312 },
	{ L'ギ', 0, 0, 240, 312 },
	{ L'グ', 0, 0, 0, 336 },
	{ L'ゲ', 0, 0, 16, 336 },
	{ L'ゴ', 0, 0, 32, 336 },
	{ L'ザ', 0, 0, 48, 336 },
	{ L'ジ', 0, 0, 64, 336 },
	{ L'ズ', 0, 0, 80, 336 },
	{ L'ゼ', 0, 0, 96, 336 },
	{ L'ゾ', 0, 0, 112, 336 },
	{ L'ダ', 0, 0, 128, 336 },
	{ L'ヂ', 0, 0, 144, 336 },
	{ L'ヅ', 0, 0, 160, 336 },
	{ L'デ', 0, 0, 176, 336 },
	{ L'ド', 0, 0, 192, 336 },
	{ L'バ', 0, 0, 208, 336 },
	{ L'ビ', 0, 0, 224, 336 },
	{ L'ブ', 0, 0, 240, 336 },
	{ L'ベ', 0, 0, 0, 360 },
	{ L'ボ', 0, 0, 16, 360 },
	{ L'パ', 0, 0, 32, 360 },
	{ L'ピ', 0, 0, 48, 360 },
	{ L'プ', 0, 0, 64, 360 },
	{ L'ペ', 0, 0, 80, 360 },
	{ L'ポ', 0, 0, 96, 360 },
	{ L'ァ', 0, 0, 112, 360 },
	{ L'ィ', 0, 0, 128, 360 },
	{ L'ゥ', 0, 0, 144, 360 },
	{ L'ェ', 0, 0, 160, 360 },
	{ L'ォ', 0, 0, 176, 360 },
	{ L'ッ', 0, 0, 192, 360 },
	{ L'ャ', 0, 0, 208, 360 },
	{ L'ュ', 0, 0, 224, 360 },
	{ L'ョ', 0, 0, 240, 360 },
};

// フォントサーフェス
SNSurfaceDDB SNBitmapFont::Font[];

// マスクサーフェス
SNSurfaceDDB SNBitmapFont::FontMask;


// 初期化処理
Void SNBitmapFont::Initialize()
{
	UInt32 cnt;
	SNImageFile* image_file;

	UInt32 font_w = BMCharWidth * BMCharBlockNumX;
	UInt32 font_h = BMCharHeight * BMCharBlockNumY;

	image_file = new SNImageFile;

	// ビットマップファイル名設定
	image_file->SetFolderFileName(SNSystemConfig::FontFileName);

	// フォントファイル読み込み
	// 非同期設定
	image_file->SetAsyncMode(true);
	image_file->Load();

	// 処理中はループする
	while (image_file->GetResult() == SNStorageResultProcessing)
	{
		// 処理が終わるまで待つ
	}


	// フォントサーフェス生成
	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Font[cnt].CreateSurface(font_w, font_h);

		// ブラシで塗りつぶし
		SNSystemBrush::Brush[cnt].Fill(&Font[cnt], 0, 0, font_w, font_h);

		// フォント描画
		Font[cnt].AndBlt(0, 0, image_file->GetDC(), 0, 0, font_w, font_h);
	}

	// マスク生成
	FontMask.CreateSurface(
		BMCharWidth * BMCharBlockNumX,
		BMCharHeight * BMCharBlockNumY);

	// フォント反転描画
	FontMask.InvertBlt(0, 0, image_file->GetDC(), 0, 0, font_w, font_h);

	delete image_file;

	return;
}

// 終了処理
Void SNBitmapFont::Terminate()
{
	UInt32 cnt;

	// フォントサーフェス破棄
	for (cnt = 0; cnt < SNColorNum; cnt++)
	{
		Font[cnt].DeleteSurface();
	}

	// マスク破棄
	FontMask.DeleteSurface();

	return;
}

// フォントサーフェス取得
SNSurface* SNBitmapFont::GetFontSurface(SNColorCode color)
{
	return &Font[color];
}

// マスクサーフェス取得
SNSurface* SNBitmapFont::GetMaskSurface()
{
	return &FontMask;
}

// 文字列描画
Void SNBitmapFont::Draw(SNSurface* dst_dc, Int32 x, Int32 y, BMString str, UInt32 len, SNColorCode color)
{
	UInt32 cnt;
	SNPoint pt;
	Int32 dst_x = x;
	Int32 dst_y = y;

	for (cnt = 0; cnt < len; cnt++)
	{
		// フォント座標取得
		pt = GetBMCharPoint(str[cnt]);

		// マスク付き転送
		dst_dc->MaskBlt(dst_x, dst_y,
			Font[color].GetDC(), pt.X, pt.Y, BMCharWidth, BMCharHeight,
			FontMask.GetDC(), pt.X, pt.Y);

		// 1文字分描画位置をずらす
		dst_x += BMCharWidth;
	}

	return;
}


// Char→BMChar変換
BMChar SNBitmapFont::CharToBMChar(Char ch)
{
	BMChar bmcode = 0x0000;

	switch (ch)
	{
	// ページ0
	// 全角変換
	case L'　': bmcode = 0x0000; break;
	case L'！': bmcode = 0x0001; break;
	case L'”': bmcode = 0x0002; break;
	case L'＃': bmcode = 0x0003; break;
	case L'＄': bmcode = 0x0004; break;
	case L'％': bmcode = 0x0005; break;
	case L'＆': bmcode = 0x0006; break;
	case L'（': bmcode = 0x0007; break;
	case L'）': bmcode = 0x0008; break;
	case L'＝': bmcode = 0x0009; break;
	case L'－': bmcode = 0x000A; break;
	case L'￥': bmcode = 0x000B; break;
	case L'＋': bmcode = 0x000C; break;
	case L'＊': bmcode = 0x000D; break;
	case L'？': bmcode = 0x000E; break;
	case L'／': bmcode = 0x000F; break;
	case L'０': bmcode = 0x0010; break;
	case L'１': bmcode = 0x0011; break;
	case L'２': bmcode = 0x0012; break;
	case L'３': bmcode = 0x0013; break;
	case L'４': bmcode = 0x0014; break;
	case L'５': bmcode = 0x0015; break;
	case L'６': bmcode = 0x0016; break;
	case L'７': bmcode = 0x0017; break;
	case L'８': bmcode = 0x0018; break;
	case L'９': bmcode = 0x0019; break;
	case L'ａ': bmcode = 0x001A; break;
	case L'ｂ': bmcode = 0x001B; break;
	case L'ｃ': bmcode = 0x001C; break;
	case L'ｄ': bmcode = 0x001D; break;
	case L'ｅ': bmcode = 0x001E; break;
	case L'ｆ': bmcode = 0x001F; break;
	case L'ｇ': bmcode = 0x0020; break;
	case L'ｈ': bmcode = 0x0021; break;
	case L'ｉ': bmcode = 0x0022; break;
	case L'ｊ': bmcode = 0x0023; break;
	case L'ｋ': bmcode = 0x0024; break;
	case L'ｌ': bmcode = 0x0025; break;
	case L'ｍ': bmcode = 0x0026; break;
	case L'ｎ': bmcode = 0x0027; break;
	case L'ｏ': bmcode = 0x0028; break;
	case L'ｐ': bmcode = 0x0029; break;
	case L'ｑ': bmcode = 0x002A; break;
	case L'ｒ': bmcode = 0x002B; break;
	case L'ｓ': bmcode = 0x002C; break;
	case L'ｔ': bmcode = 0x002D; break;
	case L'ｕ': bmcode = 0x002E; break;
	case L'ｖ': bmcode = 0x002F; break;
	case L'ｗ': bmcode = 0x0030; break;
	case L'ｘ': bmcode = 0x0031; break;
	case L'ｙ': bmcode = 0x0032; break;
	case L'ｚ': bmcode = 0x0033; break;
	case L'Ａ': bmcode = 0x0034; break;
	case L'Ｂ': bmcode = 0x0035; break;
	case L'Ｃ': bmcode = 0x0036; break;
	case L'Ｄ': bmcode = 0x0037; break;
	case L'Ｅ': bmcode = 0x0038; break;
	case L'Ｆ': bmcode = 0x0039; break;
	case L'Ｇ': bmcode = 0x003A; break;
	case L'Ｈ': bmcode = 0x003B; break;
	case L'Ｉ': bmcode = 0x003C; break;
	case L'Ｊ': bmcode = 0x003D; break;
	case L'Ｋ': bmcode = 0x003E; break;
	case L'Ｌ': bmcode = 0x003F; break;
	case L'Ｍ': bmcode = 0x0040; break;
	case L'Ｎ': bmcode = 0x0041; break;
	case L'Ｏ': bmcode = 0x0042; break;
	case L'Ｐ': bmcode = 0x0043; break;
	case L'Ｑ': bmcode = 0x0044; break;
	case L'Ｒ': bmcode = 0x0045; break;
	case L'Ｓ': bmcode = 0x0046; break;
	case L'Ｔ': bmcode = 0x0047; break;
	case L'Ｕ': bmcode = 0x0048; break;
	case L'Ｖ': bmcode = 0x0049; break;
	case L'Ｗ': bmcode = 0x004A; break;
	case L'Ｘ': bmcode = 0x004B; break;
	case L'Ｙ': bmcode = 0x004C; break;
	case L'Ｚ': bmcode = 0x004D; break;
	case L'［': bmcode = 0x004E; break;
	case L'］': bmcode = 0x004F; break;
	case L'｛': bmcode = 0x0050; break;
	case L'｝': bmcode = 0x0051; break;
	case L'「': bmcode = 0x0052; break;
	case L'」': bmcode = 0x0053; break;
	case L'＜': bmcode = 0x0054; break;
	case L'＞': bmcode = 0x0055; break;
	case L'＠': bmcode = 0x0056; break;
	case L'※': bmcode = 0x0057; break;
	case L'ー': bmcode = 0x0058; break;
	case L'、': bmcode = 0x0059; break;
	case L'。': bmcode = 0x005A; break;
	case L'，': bmcode = 0x005B; break;
	case L'．': bmcode = 0x005C; break;
	case L'・': bmcode = 0x005D; break;
	case L'’': bmcode = 0x005E; break;
	case L'ヴ': bmcode = 0x005F; break;
	case L'あ': bmcode = 0x0060; break;
	case L'い': bmcode = 0x0061; break;
	case L'う': bmcode = 0x0062; break;
	case L'え': bmcode = 0x0063; break;
	case L'お': bmcode = 0x0064; break;
	case L'か': bmcode = 0x0065; break;
	case L'き': bmcode = 0x0066; break;
	case L'く': bmcode = 0x0067; break;
	case L'け': bmcode = 0x0068; break;
	case L'こ': bmcode = 0x0069; break;
	case L'さ': bmcode = 0x006A; break;
	case L'し': bmcode = 0x006B; break;
	case L'す': bmcode = 0x006C; break;
	case L'せ': bmcode = 0x006D; break;
	case L'そ': bmcode = 0x006E; break;
	case L'た': bmcode = 0x006F; break;
	case L'ち': bmcode = 0x0070; break;
	case L'つ': bmcode = 0x0071; break;
	case L'て': bmcode = 0x0072; break;
	case L'と': bmcode = 0x0073; break;
	case L'な': bmcode = 0x0074; break;
	case L'に': bmcode = 0x0075; break;
	case L'ぬ': bmcode = 0x0076; break;
	case L'ね': bmcode = 0x0077; break;
	case L'の': bmcode = 0x0078; break;
	case L'は': bmcode = 0x0079; break;
	case L'ひ': bmcode = 0x007A; break;
	case L'ふ': bmcode = 0x007B; break;
	case L'へ': bmcode = 0x007C; break;
	case L'ほ': bmcode = 0x007D; break;
	case L'ま': bmcode = 0x007E; break;
	case L'み': bmcode = 0x007F; break;
	case L'む': bmcode = 0x0080; break;
	case L'め': bmcode = 0x0081; break;
	case L'も': bmcode = 0x0082; break;
	case L'や': bmcode = 0x0083; break;
	case L'ゆ': bmcode = 0x0084; break;
	case L'よ': bmcode = 0x0085; break;
	case L'ら': bmcode = 0x0086; break;
	case L'り': bmcode = 0x0087; break;
	case L'る': bmcode = 0x0088; break;
	case L'れ': bmcode = 0x0089; break;
	case L'ろ': bmcode = 0x008A; break;
	case L'わ': bmcode = 0x008B; break;
	case L'を': bmcode = 0x008C; break;
	case L'ん': bmcode = 0x008D; break;
	case L'が': bmcode = 0x008E; break;
	case L'ぎ': bmcode = 0x008F; break;
	case L'ぐ': bmcode = 0x0090; break;
	case L'げ': bmcode = 0x0091; break;
	case L'ご': bmcode = 0x0092; break;
	case L'ざ': bmcode = 0x0093; break;
	case L'じ': bmcode = 0x0094; break;
	case L'ず': bmcode = 0x0095; break;
	case L'ぜ': bmcode = 0x0096; break;
	case L'ぞ': bmcode = 0x0097; break;
	case L'だ': bmcode = 0x0098; break;
	case L'ぢ': bmcode = 0x0099; break;
	case L'づ': bmcode = 0x009A; break;
	case L'で': bmcode = 0x009B; break;
	case L'ど': bmcode = 0x009C; break;
	case L'ば': bmcode = 0x009D; break;
	case L'び': bmcode = 0x009E; break;
	case L'ぶ': bmcode = 0x009F; break;
	case L'べ': bmcode = 0x00A0; break;
	case L'ぼ': bmcode = 0x00A1; break;
	case L'ぱ': bmcode = 0x00A2; break;
	case L'ぴ': bmcode = 0x00A3; break;
	case L'ぷ': bmcode = 0x00A4; break;
	case L'ぺ': bmcode = 0x00A5; break;
	case L'ぽ': bmcode = 0x00A6; break;
	case L'ぁ': bmcode = 0x00A7; break;
	case L'ぃ': bmcode = 0x00A8; break;
	case L'ぅ': bmcode = 0x00A9; break;
	case L'ぇ': bmcode = 0x00AA; break;
	case L'ぉ': bmcode = 0x00AB; break;
	case L'っ': bmcode = 0x00AC; break;
	case L'ゃ': bmcode = 0x00AD; break;
	case L'ゅ': bmcode = 0x00AE; break;
	case L'ょ': bmcode = 0x00AF; break;
	case L'ア': bmcode = 0x00B0; break;
	case L'イ': bmcode = 0x00B1; break;
	case L'ウ': bmcode = 0x00B2; break;
	case L'エ': bmcode = 0x00B3; break;
	case L'オ': bmcode = 0x00B4; break;
	case L'カ': bmcode = 0x00B5; break;
	case L'キ': bmcode = 0x00B6; break;
	case L'ク': bmcode = 0x00B7; break;
	case L'ケ': bmcode = 0x00B8; break;
	case L'コ': bmcode = 0x00B9; break;
	case L'サ': bmcode = 0x00BA; break;
	case L'シ': bmcode = 0x00BB; break;
	case L'ス': bmcode = 0x00BC; break;
	case L'セ': bmcode = 0x00BD; break;
	case L'ソ': bmcode = 0x00BE; break;
	case L'タ': bmcode = 0x00BF; break;
	case L'チ': bmcode = 0x00C0; break;
	case L'ツ': bmcode = 0x00C1; break;
	case L'テ': bmcode = 0x00C2; break;
	case L'ト': bmcode = 0x00C3; break;
	case L'ナ': bmcode = 0x00C4; break;
	case L'ニ': bmcode = 0x00C5; break;
	case L'ヌ': bmcode = 0x00C6; break;
	case L'ネ': bmcode = 0x00C7; break;
	case L'ノ': bmcode = 0x00C8; break;
	case L'ハ': bmcode = 0x00C9; break;
	case L'ヒ': bmcode = 0x00CA; break;
	case L'フ': bmcode = 0x00CB; break;
	case L'ヘ': bmcode = 0x00CC; break;
	case L'ホ': bmcode = 0x00CD; break;
	case L'マ': bmcode = 0x00CE; break;
	case L'ミ': bmcode = 0x00CF; break;
	case L'ム': bmcode = 0x00D0; break;
	case L'メ': bmcode = 0x00D1; break;
	case L'モ': bmcode = 0x00D2; break;
	case L'ヤ': bmcode = 0x00D3; break;
	case L'ユ': bmcode = 0x00D4; break;
	case L'ヨ': bmcode = 0x00D5; break;
	case L'ラ': bmcode = 0x00D6; break;
	case L'リ': bmcode = 0x00D7; break;
	case L'ル': bmcode = 0x00D8; break;
	case L'レ': bmcode = 0x00D9; break;
	case L'ロ': bmcode = 0x00DA; break;
	case L'ワ': bmcode = 0x00DB; break;
	case L'ヲ': bmcode = 0x00DC; break;
	case L'ン': bmcode = 0x00DD; break;
	case L'ガ': bmcode = 0x00DE; break;
	case L'ギ': bmcode = 0x00DF; break;
	case L'グ': bmcode = 0x00E0; break;
	case L'ゲ': bmcode = 0x00E1; break;
	case L'ゴ': bmcode = 0x00E2; break;
	case L'ザ': bmcode = 0x00E3; break;
	case L'ジ': bmcode = 0x00E4; break;
	case L'ズ': bmcode = 0x00E5; break;
	case L'ゼ': bmcode = 0x00E6; break;
	case L'ゾ': bmcode = 0x00E7; break;
	case L'ダ': bmcode = 0x00E8; break;
	case L'ヂ': bmcode = 0x00E9; break;
	case L'ヅ': bmcode = 0x00EA; break;
	case L'デ': bmcode = 0x00EB; break;
	case L'ド': bmcode = 0x00EC; break;
	case L'バ': bmcode = 0x00ED; break;
	case L'ビ': bmcode = 0x00EE; break;
	case L'ブ': bmcode = 0x00EF; break;
	case L'ベ': bmcode = 0x00F0; break;
	case L'ボ': bmcode = 0x00F1; break;
	case L'パ': bmcode = 0x00F2; break;
	case L'ピ': bmcode = 0x00F3; break;
	case L'プ': bmcode = 0x00F4; break;
	case L'ペ': bmcode = 0x00F5; break;
	case L'ポ': bmcode = 0x00F6; break;
	case L'ァ': bmcode = 0x00F7; break;
	case L'ィ': bmcode = 0x00F8; break;
	case L'ゥ': bmcode = 0x00F9; break;
	case L'ェ': bmcode = 0x00FA; break;
	case L'ォ': bmcode = 0x00FB; break;
	case L'ッ': bmcode = 0x00FC; break;
	case L'ャ': bmcode = 0x00FD; break;
	case L'ュ': bmcode = 0x00FE; break;
	case L'ョ': bmcode = 0x00FF; break;

	// ページ0
	// 半角変換
	case L' ': bmcode = 0x0000; break;
	case L'!': bmcode = 0x0001; break;
	case L'"': bmcode = 0x0002; break;
	case L'#': bmcode = 0x0003; break;
	case L'$': bmcode = 0x0004; break;
	case L'%': bmcode = 0x0005; break;
	case L'&': bmcode = 0x0006; break;
	case L'(': bmcode = 0x0007; break;
	case L')': bmcode = 0x0008; break;
	case L'=': bmcode = 0x0009; break;
	case L'-': bmcode = 0x000A; break;
	case L'\\': bmcode = 0x000B; break;
	case L'+': bmcode = 0x000C; break;
	case L'*': bmcode = 0x000D; break;
	case L'?': bmcode = 0x000E; break;
	case L'/': bmcode = 0x000F; break;
	case L'0': bmcode = 0x0010; break;
	case L'1': bmcode = 0x0011; break;
	case L'2': bmcode = 0x0012; break;
	case L'3': bmcode = 0x0013; break;
	case L'4': bmcode = 0x0014; break;
	case L'5': bmcode = 0x0015; break;
	case L'6': bmcode = 0x0016; break;
	case L'7': bmcode = 0x0017; break;
	case L'8': bmcode = 0x0018; break;
	case L'9': bmcode = 0x0019; break;
	case L'a': bmcode = 0x001A; break;
	case L'b': bmcode = 0x001B; break;
	case L'c': bmcode = 0x001C; break;
	case L'd': bmcode = 0x001D; break;
	case L'e': bmcode = 0x001E; break;
	case L'f': bmcode = 0x001F; break;
	case L'g': bmcode = 0x0020; break;
	case L'h': bmcode = 0x0021; break;
	case L'i': bmcode = 0x0022; break;
	case L'j': bmcode = 0x0023; break;
	case L'k': bmcode = 0x0024; break;
	case L'l': bmcode = 0x0025; break;
	case L'm': bmcode = 0x0026; break;
	case L'n': bmcode = 0x0027; break;
	case L'o': bmcode = 0x0028; break;
	case L'p': bmcode = 0x0029; break;
	case L'q': bmcode = 0x002A; break;
	case L'r': bmcode = 0x002B; break;
	case L's': bmcode = 0x002C; break;
	case L't': bmcode = 0x002D; break;
	case L'u': bmcode = 0x002E; break;
	case L'v': bmcode = 0x002F; break;
	case L'w': bmcode = 0x0030; break;
	case L'x': bmcode = 0x0031; break;
	case L'y': bmcode = 0x0032; break;
	case L'z': bmcode = 0x0033; break;
	case L'A': bmcode = 0x0034; break;
	case L'B': bmcode = 0x0035; break;
	case L'C': bmcode = 0x0036; break;
	case L'D': bmcode = 0x0037; break;
	case L'E': bmcode = 0x0038; break;
	case L'F': bmcode = 0x0039; break;
	case L'G': bmcode = 0x003A; break;
	case L'H': bmcode = 0x003B; break;
	case L'I': bmcode = 0x003C; break;
	case L'J': bmcode = 0x003D; break;
	case L'K': bmcode = 0x003E; break;
	case L'L': bmcode = 0x003F; break;
	case L'M': bmcode = 0x0040; break;
	case L'N': bmcode = 0x0041; break;
	case L'O': bmcode = 0x0042; break;
	case L'P': bmcode = 0x0043; break;
	case L'Q': bmcode = 0x0044; break;
	case L'R': bmcode = 0x0045; break;
	case L'S': bmcode = 0x0046; break;
	case L'T': bmcode = 0x0047; break;
	case L'U': bmcode = 0x0048; break;
	case L'V': bmcode = 0x0049; break;
	case L'W': bmcode = 0x004A; break;
	case L'X': bmcode = 0x004B; break;
	case L'Y': bmcode = 0x004C; break;
	case L'Z': bmcode = 0x004D; break;
	case L'[': bmcode = 0x004E; break;
	case L']': bmcode = 0x004F; break;
	case L'{': bmcode = 0x0050; break;
	case L'}': bmcode = 0x0051; break;
	case L'<': bmcode = 0x0054; break;
	case L'>': bmcode = 0x0055; break;
	case L'@': bmcode = 0x0056; break;
	case L',': bmcode = 0x005B; break;
	case L'.': bmcode = 0x005C; break;
	case L'･': bmcode = 0x005D; break;
	case L'\'': bmcode = 0x005E; break;
	}

	return bmcode;
}

// String→BMString
Void SNBitmapFont::StringToBMString(String str, Int32 len, BMString bmstr, Int32 bmlen)
{
	Int32 cnt;
	Int32 cnv_len = (Int32)SNMath::SelectMin(len, bmlen);

	// 変換文字数分、Char→BMChar変換
	for (cnt = 0; cnt < cnv_len; cnt++)
	{
		bmstr[cnt] = CharToBMChar(str[cnt]);
	}

	return;
}

// BMChar→Char変換
Char SNBitmapFont::BMCharToChar(BMChar bmch)
{
	return BMCharTable[bmch].Character;
}

// BMString→String変換
Void SNBitmapFont::BMStringToString(BMString bmstr, Int32 bmlen, String str, Int32 len)
{
	Int32 cnt;
	Int32 cnv_len = (Int32)SNMath::SelectMin(len, bmlen);

	// 変換文字数分、BMChar→Char変換
	for (cnt = 0; cnt < cnv_len; cnt++)
	{
		str[cnt] = BMCharToChar(bmstr[cnt]);
	}

	return;
}

// ビットマップフォント座標取得
SNPoint SNBitmapFont::GetBMCharPoint(BMChar bmch)
{
	return BMCharTable[bmch].Point;
}
