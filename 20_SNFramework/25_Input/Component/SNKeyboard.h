#pragma once
#include "SNFrameworkInternal.h"

// キーボードクラス
// システム設定で指定したサポートキーに対して押下状態の監視を行う
// 各キーに対しての押下状態を保持する

class SNKeyboard
{
public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 状態更新
	static Void Update();

	// キー状態
	static Boolean KeyState[SNKeyCodeNum];

private:
	// サポートキーリスト
	static SNKeyCode SupportKeyList[SNKeyCodeNum];

	// サポートキー数
	static UInt8 SupportKeyNum;

	// キーサポート情報
	// True = サポート、False = 非サポート
	static constexpr Boolean KeySupportInfo[SNKeyCodeNum] =
	{
		false,  // 0x00:null
		false,  // 0x01:マウスの左ボタン
		false,  // 0x02:マウスの右ボタン
		false,  // 0x03:制御中断処理
		false,  // 0x04:マウスの中央ボタン
		false,  // 0x05:X1 マウス ボタン
		false,  // 0x06:X2 マウス ボタン
		false,  // 0x07:予約済み
		true,  // 0x08:Backspace キー
		true,  // 0x09:Tab キー
		false,  // 0x0A:予約済み
		false,  // 0x0B:予約済み
		true,  // 0x0C:Clear キー
		true,  // 0x0D:Enter キー
		false,  // 0x0E:[Unassigned] \(未割り当て)
		false,  // 0x0F:[Unassigned] \(未割り当て)
		true,  // 0x10:Shift キー
		true,  // 0x11:Ctrl キー
		true,  // 0x12:ALT キー
		true,  // 0x13:Pause キー
		true,  // 0x14:CAPS LOCK キー
		true,  // 0x15:IME かなモード
		true,  // 0x16:IME オン
		true,  // 0x17:IME Junja モード
		true,  // 0x18:IME Final モード
		true,  // 0x19:IME Hanja モード
		true,  // 0x1A:IME オフ
		true,  // 0x1B:Esc キー
		true,  // 0x1C:IME 変換
		true,  // 0x1D:IME 無変換
		true,  // 0x1E:IME 使用可能
		true,  // 0x1F:IME モード変更要求
		true,  // 0x20:Space キー
		true,  // 0x21:PageUp キー
		true,  // 0x22:PageDown キー
		true,  // 0x23:End キー
		true,  // 0x24:Home キー
		true,  // 0x25:左方向キー
		true,  // 0x26:上方向キー
		true,  // 0x27:右方向キー
		true,  // 0x28:下方向キー
		true,  // 0x29:Select キー
		true,  // 0x2A:Print キー
		true,  // 0x2B:Execute キー
		true,  // 0x2C:Print Screen キー
		true,  // 0x2D:Ins キー
		true,  // 0x2E:DEL キー
		true,  // 0x2F:Help キー
		true,  // 0x30:0 キー
		true,  // 0x31:1 キー
		true,  // 0x32:2 キー
		true,  // 0x33:3 キー
		true,  // 0x34:4 キー
		true,  // 0x35:5 キー
		true,  // 0x36:6 キー
		true,  // 0x37:7 キー
		true,  // 0x38:8 キー
		true,  // 0x39:9 キー
		false,  // 0x3A:未定義。
		false,  // 0x3B:未定義。
		false,  // 0x3C:未定義。
		false,  // 0x3D:未定義。
		false,  // 0x3E:未定義。
		false,  // 0x3F:未定義。
		false,  // 0x40:未定義。
		true,  // 0x41:A キー
		true,  // 0x42:B キー
		true,  // 0x43:C キー
		true,  // 0x44:D キー
		true,  // 0x45:E キー
		true,  // 0x46:F キー
		true,  // 0x47:G キー
		true,  // 0x48:H キー
		true,  // 0x49:I キー
		true,  // 0x4A:J キー
		true,  // 0x4B:K キー
		true,  // 0x4C:L キー
		true,  // 0x4D:M キー
		true,  // 0x4E:N キー
		true,  // 0x4F:O キー
		true,  // 0x50:P キー
		true,  // 0x51:Q キー
		true,  // 0x52:R キー
		true,  // 0x53:S キー
		true,  // 0x54:T キー
		true,  // 0x55:U キー
		true,  // 0x56:V キー
		true,  // 0x57:W キー
		true,  // 0x58:X キー
		true,  // 0x59:Y キー
		true,  // 0x5A:Z キー
		true,  // 0x5B:Windows の左キー
		true,  // 0x5C:右の Windows キー
		true,  // 0x5D:アプリケーション キー
		false,  // 0x5E:予約済み
		true,  // 0x5F:コンピューターのスリープ キー
		true,  // 0x60:テンキーの 0 キー
		true,  // 0x61:テンキーの 1 キー
		true,  // 0x62:テンキーの 2 キー
		true,  // 0x63:テンキーの 3 キー
		true,  // 0x64:テンキーの 4 キー
		true,  // 0x65:テンキーの 5 キー
		true,  // 0x66:テンキーの 6 キー
		true,  // 0x67:テンキーの 7 キー
		true,  // 0x68:テンキーの 8 キー
		true,  // 0x69:テンキーの 9 キー
		true,  // 0x6A:乗算キー
		true,  // 0x6B:キーの追加
		true,  // 0x6C:区切り記号キー
		true,  // 0x6D:減算キー
		true,  // 0x6E:10 進キー
		true,  // 0x6F:除算キー
		true,  // 0x70:F1 キー
		true,  // 0x71:F2 キー
		true,  // 0x72:F3 キー
		true,  // 0x73:F4 キー
		true,  // 0x74:F5 キー
		true,  // 0x75:F6 キー
		true,  // 0x76:F7 キー
		true,  // 0x77:F8 キー
		true,  // 0x78:F9 キー
		true,  // 0x79:F10 キー
		true,  // 0x7A:F11 キー
		true,  // 0x7B:F12 キー
		true,  // 0x7C:F13 キー
		true,  // 0x7D:F14 キー
		true,  // 0x7E:F15 キー
		true,  // 0x7F:F16 キー
		true,  // 0x80:F17 キー
		true,  // 0x81:F18 キー
		true,  // 0x82:F19 キー
		true,  // 0x83:F20 キー
		true,  // 0x84:F21 キー
		true,  // 0x85:F22 キー
		true,  // 0x86:F23 キー
		true,  // 0x87:F24 キー
		false,  // 0x88:予約済み
		false,  // 0x89:予約済み
		false,  // 0x8A:予約済み
		false,  // 0x8B:予約済み
		false,  // 0x8C:予約済み
		false,  // 0x8D:予約済み
		false,  // 0x8E:予約済み
		false,  // 0x8F:予約済み
		true,  // 0x90:NUM LOCK キー
		true,  // 0x91:ScrollLock キー
		false,  // 0x92:OEM 固有
		false,  // 0x93:OEM 固有
		false,  // 0x94:OEM 固有
		false,  // 0x95:OEM 固有
		false,  // 0x96:OEM 固有
		false,  // 0x97:[Unassigned] \(未割り当て)
		false,  // 0x98:[Unassigned] \(未割り当て)
		false,  // 0x99:[Unassigned] \(未割り当て)
		false,  // 0x9A:[Unassigned] \(未割り当て)
		false,  // 0x9B:[Unassigned] \(未割り当て)
		false,  // 0x9C:[Unassigned] \(未割り当て)
		false,  // 0x9D:[Unassigned] \(未割り当て)
		false,  // 0x9E:[Unassigned] \(未割り当て)
		false,  // 0x9F:[Unassigned] \(未割り当て)
		true,  // 0xA0:左 Shift キー
		true,  // 0xA1:右 Shift キー
		true,  // 0xA2:左 Ctrl キー
		true,  // 0xA3:右 Ctrl キー
		true,  // 0xA4:左 Alt キー
		true,  // 0xA5:右 Alt キー
		true,  // 0xA6:ブラウザーの戻るキー
		true,  // 0xA7:ブラウザーの進むキー
		true,  // 0xA8:ブラウザーの更新キー
		true,  // 0xA9:ブラウザーの停止キー
		true,  // 0xAA:ブラウザーの検索キー
		true,  // 0xAB:ブラウザーのお気に入りキー
		true,  // 0xAC:ブラウザーのスタートとホーム キー
		true,  // 0xAD:音量ミュート キー
		true,  // 0xAE:音量下げるキー
		true,  // 0xAF:音量上げるキー
		true,  // 0xB0:次のトラックキー
		true,  // 0xB1:前のトラック
		true,  // 0xB2:メディアの停止キー
		true,  // 0xB3:メディアの再生/一時停止キー
		true,  // 0xB4:メール開始キー
		true,  // 0xB5:メディアの選択キー
		true,  // 0xB6:アプリケーション 1 の起動キー
		true,  // 0xB7:アプリケーション 2 の起動キー
		false,  // 0xB8:予約済み
		false,  // 0xB9:予約済み
		true,  // 0xBA:その他の文字に使用されます。
		true,  // 0xBB:どの国/地域の場合でも+キー
		true,  // 0xBC:どの国/地域の場合でも,キー
		true,  // 0xBD:どの国/地域の場合でも-キー
		true,  // 0xBE:どの国/地域の場合でも.キー
		true,  // 0xBF:その他の文字に使用されます。
		true,  // 0xC0:その他の文字に使用されます。
		false,  // 0xC1:予約済み
		false,  // 0xC2:予約済み
		false,  // 0xC3:予約済み
		false,  // 0xC4:予約済み
		false,  // 0xC5:予約済み
		false,  // 0xC6:予約済み
		false,  // 0xC7:予約済み
		false,  // 0xC8:予約済み
		false,  // 0xC9:予約済み
		false,  // 0xCA:予約済み
		false,  // 0xCB:予約済み
		false,  // 0xCC:予約済み
		false,  // 0xCD:予約済み
		false,  // 0xCE:予約済み
		false,  // 0xCF:予約済み
		false,  // 0xD0:予約済み
		false,  // 0xD1:予約済み
		false,  // 0xD2:予約済み
		false,  // 0xD3:予約済み
		false,  // 0xD4:予約済み
		false,  // 0xD5:予約済み
		false,  // 0xD6:予約済み
		false,  // 0xD7:予約済み
		false,  // 0xD8:予約済み
		false,  // 0xD9:予約済み
		false,  // 0xDA:予約済み
		true,  // 0xDB:その他の文字に使用されます。
		true,  // 0xDC:その他の文字に使用されます。
		true,  // 0xDD:その他の文字に使用されます。
		true,  // 0xDE:その他の文字に使用されます。
		true,  // 0xDF:その他の文字に使用されます。
		false,  // 0xE0:予約済み
		false,  // 0xE1:OEM 固有
		true,  // 0xE2:標準的な US キーボードの<>キー
		false,  // 0xE3:OEM 固有
		false,  // 0xE4:OEM 固有
		true,  // 0xE5:IME PROCESS キー
		false,  // 0xE6:OEM 固有
		false,  // 0xE7:キーボード以外の入力手段に使用される 32 ビット仮想キー値の下位ワードです。 
		false,  // 0xE8:[Unassigned] \(未割り当て)
		false,  // 0xE9:OEM 固有
		false,  // 0xEA:OEM 固有
		false,  // 0xEB:OEM 固有
		false,  // 0xEC:OEM 固有
		false,  // 0xED:OEM 固有
		false,  // 0xEE:OEM 固有
		false,  // 0xEF:OEM 固有
		false,  // 0xF0:OEM 固有
		false,  // 0xF1:OEM 固有
		false,  // 0xF2:OEM 固有
		false,  // 0xF3:OEM 固有
		false,  // 0xF4:OEM 固有
		false,  // 0xF5:OEM 固有
		true,  // 0xF6:Attn キー
		true,  // 0xF7:CrSel キー
		true,  // 0xF8:ExSel キー
		true,  // 0xF9:EOF 消去キー
		true,  // 0xFA:再生キー
		true,  // 0xFB:ズーム キー
		true,  // 0xFC:予約済み
		true,  // 0xFD:PA1 キー
		true,  // 0xFE:クリア キー
		false  // 0xFF:-
	};
};
