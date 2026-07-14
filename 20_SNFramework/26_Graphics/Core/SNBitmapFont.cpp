#include "SNBitmapFont.h"
#include "SNGraphicsDevice.h"
#include "SNWindowsAPI.h"
#include "SNMath.h"
#include "SNConfig.h"
#include "SNGraphics.h"
#include "SNImageFile.h"
#include "SNGraphicsContext.h"

// ビットマップフォントクラス

// ビットマップフォントテーブル
const Char SNBitmapFont::BMCharTable[] =
{
L'　',	L'！',	L'”',	L'＃',	L'＄',	L'％',	L'＆',	L'（',	L'）',	L'＝',	L'－',	L'￥',	L'＋',	L'＊',	L'？',	L'／',
L'０',	L'１',	L'２',	L'３',	L'４',	L'５',	L'６',	L'７',	L'８',	L'９',	L'ａ',	L'ｂ',	L'ｃ',	L'ｄ',	L'ｅ',	L'ｆ',
L'ｇ',	L'ｈ',	L'ｉ',	L'ｊ',	L'ｋ',	L'ｌ',	L'ｍ',	L'ｎ',	L'ｏ',	L'ｐ',	L'ｑ',	L'ｒ',	L'ｓ',	L'ｔ',	L'ｕ',	L'ｖ',
L'ｗ',	L'ｘ',	L'ｙ',	L'ｚ',	L'Ａ',	L'Ｂ',	L'Ｃ',	L'Ｄ',	L'Ｅ',	L'Ｆ',	L'Ｇ',	L'Ｈ',	L'Ｉ',	L'Ｊ',	L'Ｋ',	L'Ｌ',
L'Ｍ',	L'Ｎ',	L'Ｏ',	L'Ｐ',	L'Ｑ',	L'Ｒ',	L'Ｓ',	L'Ｔ',	L'Ｕ',	L'Ｖ',	L'Ｗ',	L'Ｘ',	L'Ｙ',	L'Ｚ',	L'［',	L'］',
L'｛',	L'｝',	L'「',	L'」',	L'＜',	L'＞',	L'＠',	L'※',	L'ー',	L'、',	L'。',	L'，',	L'．',	L'・',	L'’',	L'ヴ',
L'あ',	L'い',	L'う',	L'え',	L'お',	L'か',	L'き',	L'く',	L'け',	L'こ',	L'さ',	L'し',	L'す',	L'せ',	L'そ',	L'た',
L'ち',	L'つ',	L'て',	L'と',	L'な',	L'に',	L'ぬ',	L'ね',	L'の',	L'は',	L'ひ',	L'ふ',	L'へ',	L'ほ',	L'ま',	L'み',
L'む',	L'め',	L'も',	L'や',	L'ゆ',	L'よ',	L'ら',	L'り',	L'る',	L'れ',	L'ろ',	L'わ',	L'を',	L'ん',	L'が',	L'ぎ',
L'ぐ',	L'げ',	L'ご',	L'ざ',	L'じ',	L'ず',	L'ぜ',	L'ぞ',	L'だ',	L'ぢ',	L'づ',	L'で',	L'ど',	L'ば',	L'び',	L'ぶ',
L'べ',	L'ぼ',	L'ぱ',	L'ぴ',	L'ぷ',	L'ぺ',	L'ぽ',	L'ぁ',	L'ぃ',	L'ぅ',	L'ぇ',	L'ぉ',	L'っ',	L'ゃ',	L'ゅ',	L'ょ',
L'ア',	L'イ',	L'ウ',	L'エ',	L'オ',	L'カ',	L'キ',	L'ク',	L'ケ',	L'コ',	L'サ',	L'シ',	L'ス',	L'セ',	L'ソ',	L'タ',
L'チ',	L'ツ',	L'テ',	L'ト',	L'ナ',	L'ニ',	L'ヌ',	L'ネ',	L'ノ',	L'ハ',	L'ヒ',	L'フ',	L'ヘ',	L'ホ',	L'マ',	L'ミ',
L'ム',	L'メ',	L'モ',	L'ヤ',	L'ユ',	L'ヨ',	L'ラ',	L'リ',	L'ル',	L'レ',	L'ロ',	L'ワ',	L'ヲ',	L'ン',	L'ガ',	L'ギ',
L'グ',	L'ゲ',	L'ゴ',	L'ザ',	L'ジ',	L'ズ',	L'ゼ',	L'ゾ',	L'ダ',	L'ヂ',	L'ヅ',	L'デ',	L'ド',	L'バ',	L'ビ',	L'ブ',
L'ベ',	L'ボ',	L'パ',	L'ピ',	L'プ',	L'ペ',	L'ポ',	L'ァ',	L'ィ',	L'ゥ',	L'ェ',	L'ォ',	L'ッ',	L'ャ',	L'ュ',	L'ョ',
};

// フォントサーフェス
SNBitmap SNBitmapFont::FontSurface[];


// 初期化処理
Void SNBitmapFont::Initialize()
{
	SNImageFile image_file;
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;

	// ビットマップファイル名設定
	image_file.SetFolderFileName(SNSystemConfig::FontFileName);

	// フォントファイル読み込み
	image_file.Read();

	// 読み込み完了待ち
	image_file.WaitForOperationComplete();

	// デコード
	image_file.Decode();

	// デコード完了待ち
	image_file.WaitForOperationComplete();

	// デコード結果からフォントサーフェス生成
	grc->CreateBitmapFromDIB(&image_file, &FontSurface[0]);


// テストコード

	SNImageFile image_file2;
	SNSize snsize;

	image_file.GetSize(&snsize);

	image_file2.SetFolderFileName((String)L"ビットマップフォント2.png");

	grc->CreateDIBFromBitmap(&FontSurface[0], &image_file2);

	image_file2.Encode();
	image_file2.WaitForOperationComplete();

	image_file2.Write();
	image_file2.WaitForOperationComplete();
	
	return;
}

// 終了処理
Void SNBitmapFont::Terminate()
{
	UInt32 cnt;

	// フォントサーフェス破棄
	for (cnt = 0; cnt < BMPageNum; cnt++)
	{
		FontSurface[cnt].DeleteBitmap();
	}

	return;
}

// 文字列描画
Void SNBitmapFont::Draw(SNGraphicsContext* dst_dc, Int32 x, Int32 y, BMString str, UInt32 len, SNColorCode color)
{
	UInt32 cnt;
	SNPoint pt;
	SNRect dst_rect;
	SNRect src_rect;
	UInt8 page;

	dst_rect.PointX = x;
	dst_rect.PointY = y;
	dst_rect.Width = BMCharWidth;
	dst_rect.Height = BMCharHeight;
	src_rect.Width = BMCharWidth;
	src_rect.Height = BMCharHeight;

	for (cnt = 0; cnt < len; cnt++)
	{
		// フォント座標取得
		pt = GetBMCharPoint(str[cnt]);

		src_rect.PointX = pt.X;
		src_rect.PointY = pt.Y;

		page = GetBMCharPage(str[cnt]);

		// 描画
		dst_dc->DrawImage(&dst_rect, &FontSurface[page], &src_rect, SNAlphaMax);

		// 1文字分描画位置をずらす
		dst_rect.PointX += BMCharWidth;
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
	return BMCharTable[bmch];
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
	SNPoint pnt;
	UInt32 code = (bmch & 0x00FF);

	pnt.X = (code % BMCharBlockNumX) * BMCharWidth;
	pnt.Y = (code / BMCharBlockNumY) * BMCharHeight;

	return pnt;
}

// ビットマップフォントページ取得
UInt8 SNBitmapFont::GetBMCharPage(BMChar bmch)
{
	UInt8 page = (((bmch & 0xFF00) >> 8) & 0x00FF);

	return page;
}
