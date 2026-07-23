#pragma once

///////////////////////////////////////////////////////////////
// システム/アプリケーション関連

// BLTモード
enum SNBltMode
{
    SNBltModeAnd = 1,
    SNBltModeOr = 2,
    SNBltModeNearest = 3,
    SNBltModeHalfTone = 4
};


// アプリケーションイベント
enum SNApplicationEvent
{
    SNEventActive = 0,		// アクティブ通知
    SNEventNonActive,		// 非アクティブ通知
    SNEventExitApplication,	// 終了通知
    SNEventWheelUp,		    // ホイールUp通知
    SNEventWheelDown,		// ホイールDown通知
    SNEventNormalDisp,      // 通常表示通知
    SNEventMaximizeDisp,    // 最大化通知
    SNEventWindowSize,      // ウインドウサイズ変更通知
    SNEventNum              // イベント数
};

// 遷移先コード
// -1:遷移なし
// 0~:状態クラス毎に規程する
enum SNTransitionCode
{
    SNTransitionCodeStay = -1,
    SNTransitionCodeTop = 0,
    SNTransitionCode0 = SNTransitionCodeTop,
    SNTransitionCode1 = 1,
    SNTransitionCode2 = 2,
    SNTransitionCode3 = 3,
    SNTransitionCodeLast = SNTransitionCode3,
    SNTransitionCodeNum,
};


///////////////////////////////////////////////////////////////
// 実入力デバイス関連

// 実入力デバイス種別
enum SNInputDeviceType
{
    SNInputTypeKeyboard,          // キーボード
    SNInputTypeMouse,             // マウス
    SNInputTypeGamePad1,          // ゲームパッド1
    SNInputTypeGamePad2,          // ゲームパッド2
    SNInputTypeScreenKeyboard,    // スクリーンキーボード
    SNInputTypeScreenGamePad,     // スクリーンゲームパッド
    SNInputTypeNum
};

// キーコード
enum SNKeyCode
{
    SNKeyCodeNull = 0x00,
    SNKeyCodeMouseLeft = 0x01,
    SNKeyCodeMouseRight = 0x02,
    SNKeyCodeInterrupt = 0x03,
    SNKeyCodeMouseMiddle = 0x04,
    SNKeyCodeMouseX1 = 0x05,
    SNKeyCodeMouseX2 = 0x06,
    SNKeyCodeReserved07 = 0x07,
    SNKeyCodeBackspace = 0x08,
    SNKeyCodeTab = 0x09,
    SNKeyCodeReserved0A = 0x0A,
    SNKeyCodeReserved0B = 0x0B,
    SNKeyCodeClear = 0x0C,
    SNKeyCodeEnter = 0x0D,
    SNKeyCodeUnassigned0E = 0x0E,
    SNKeyCodeUnassigned0F = 0x0F,
    SNKeyCodeShift = 0x10,
    SNKeyCodeCtrl = 0x11,
    SNKeyCodeAlt = 0x12,
    SNKeyCodePause = 0x13,
    SNKeyCodeCapslock = 0x14,
    SNKeyCodeImeKana = 0x15,
    SNKeyCodeImeOn = 0x16,
    SNKeyCodeImeJunja = 0x17,
    SNKeyCodeImeFinal = 0x18,
    SNKeyCodeImeHanja = 0x19,
    SNKeyCodeImeOff = 0x1A,
    SNKeyCodeEsc = 0x1B,
    SNKeyCodeImeConvert = 0x1C,
    SNKeyCodeImeNonconvert = 0x1D,
    SNKeyCodeImeAccept = 0x1E,
    SNKeyCodeImeModechange = 0x1F,
    SNKeyCodeSpace = 0x20,
    SNKeyCodePageUp = 0x21,
    SNKeyCodePageDown = 0x22,
    SNKeyCodeEnd = 0x23,
    SNKeyCodeHome = 0x24,
    SNKeyCodeLeft = 0x25,
    SNKeyCodeUp = 0x26,
    SNKeyCodeRight = 0x27,
    SNKeyCodeDown = 0x28,
    SNKeyCodeSelect = 0x29,
    SNKeyCodePrint = 0x2A,
    SNKeyCodeExecute = 0x2B,
    SNKeyCodeSnapshot = 0x2C,
    SNKeyCodeInsert = 0x2D,
    SNKeyCodeDelete = 0x2E,
    SNKeyCodeHelp = 0x2F,
    SNKeyCode0 = 0x30,
    SNKeyCode1 = 0x31,
    SNKeyCode2 = 0x32,
    SNKeyCode3 = 0x33,
    SNKeyCode4 = 0x34,
    SNKeyCode5 = 0x35,
    SNKeyCode6 = 0x36,
    SNKeyCode7 = 0x37,
    SNKeyCode8 = 0x38,
    SNKeyCode9 = 0x39,
    SNKeyCodeReserved3A = 0x3A,
    SNKeyCodeReserved3B = 0x3B,
    SNKeyCodeReserved3C = 0x3C,
    SNKeyCodeReserved3D = 0x3D,
    SNKeyCodeReserved3E = 0x3E,
    SNKeyCodeReserved3F = 0x3F,
    SNKeyCodeReserved40 = 0x40,
    SNKeyCodeA = 0x41,
    SNKeyCodeB = 0x42,
    SNKeyCodeC = 0x43,
    SNKeyCodeD = 0x44,
    SNKeyCodeE = 0x45,
    SNKeyCodeF = 0x46,
    SNKeyCodeG = 0x47,
    SNKeyCodeH = 0x48,
    SNKeyCodeI = 0x49,
    SNKeyCodeJ = 0x4A,
    SNKeyCodeK = 0x4B,
    SNKeyCodeL = 0x4C,
    SNKeyCodeM = 0x4D,
    SNKeyCodeN = 0x4E,
    SNKeyCodeO = 0x4F,
    SNKeyCodeP = 0x50,
    SNKeyCodeQ = 0x51,
    SNKeyCodeR = 0x52,
    SNKeyCodeS = 0x53,
    SNKeyCodeT = 0x54,
    SNKeyCodeU = 0x55,
    SNKeyCodeV = 0x56,
    SNKeyCodeW = 0x57,
    SNKeyCodeX = 0x58,
    SNKeyCodeY = 0x59,
    SNKeyCodeZ = 0x5A,
    SNKeyCodeLwin = 0x5B,
    SNKeyCodeRwin = 0x5C,
    SNKeyCodeApps = 0x5D,
    SNKeyCodeReserved5E = 0x5E,
    SNKeyCodeSleep = 0x5F,
    SNKeyCodeNumpad0 = 0x60,
    SNKeyCodeNumpad1 = 0x61,
    SNKeyCodeNumpad2 = 0x62,
    SNKeyCodeNumpad3 = 0x63,
    SNKeyCodeNumpad4 = 0x64,
    SNKeyCodeNumpad5 = 0x65,
    SNKeyCodeNumpad6 = 0x66,
    SNKeyCodeNumpad7 = 0x67,
    SNKeyCodeNumpad8 = 0x68,
    SNKeyCodeNumpad9 = 0x69,
    SNKeyCodeMultiply = 0x6A,
    SNKeyCodeAdd = 0x6B,
    SNKeyCodeSeparator = 0x6C,
    SNKeyCodeSubtract = 0x6D,
    SNKeyCodeDecimal = 0x6E,
    SNKeyCodeDivide = 0x6F,
    SNKeyCodeF1 = 0x70,
    SNKeyCodeF2 = 0x71,
    SNKeyCodeF3 = 0x72,
    SNKeyCodeF4 = 0x73,
    SNKeyCodeF5 = 0x74,
    SNKeyCodeF6 = 0x75,
    SNKeyCodeF7 = 0x76,
    SNKeyCodeF8 = 0x77,
    SNKeyCodeF9 = 0x78,
    SNKeyCodeF10 = 0x79,
    SNKeyCodeF11 = 0x7A,
    SNKeyCodeF12 = 0x7B,
    SNKeyCodeF13 = 0x7C,
    SNKeyCodeF14 = 0x7D,
    SNKeyCodeF15 = 0x7E,
    SNKeyCodeF16 = 0x7F,
    SNKeyCodeF17 = 0x80,
    SNKeyCodeF18 = 0x81,
    SNKeyCodeF19 = 0x82,
    SNKeyCodeF20 = 0x83,
    SNKeyCodeF21 = 0x84,
    SNKeyCodeF22 = 0x85,
    SNKeyCodeF23 = 0x86,
    SNKeyCodeReserved87 = 0x87,
    SNKeyCodeReserved88 = 0x88,
    SNKeyCodeReserved89 = 0x89,
    SNKeyCodeReserved8A = 0x8A,
    SNKeyCodeReserved8B = 0x8B,
    SNKeyCodeReserved8C = 0x8C,
    SNKeyCodeReserved8D = 0x8D,
    SNKeyCodeReserved8E = 0x8E,
    SNKeyCodeReserved8F = 0x8F,
    SNKeyCodeNumlock = 0x90,
    SNKeyCodeScrolllock = 0x91,
    SNKeyCodeOemNecEqual = 0x92,
    SNKeyCodeOemFjJisho = 0x93,
    SNKeyCodeOemFjMasshou = 0x94,
    SNKeyCodeOemFjTouroku = 0x95,
    SNKeyCodeOemFjLoya = 0x96,
    SNKeyCodeOemFjRoya = 0x97,
    SNKeyCodeReserved98 = 0x98,
    SNKeyCodeReserved99 = 0x99,
    SNKeyCodeReserved9A = 0x9A,
    SNKeyCodeReserved9B = 0x9B,
    SNKeyCodeReserved9C = 0x9C,
    SNKeyCodeReserved9D = 0x9D,
    SNKeyCodeLshift = 0xA0,
    SNKeyCodeRshift = 0xA1,
    SNKeyCodeLctrl = 0xA2,
    SNKeyCodeRctrl = 0xA3,
    SNKeyCodeLalt = 0xA4,
    SNKeyCodeRalt = 0xA5,
    SNKeyCodeBrowserBack = 0xA6,
    SNKeyCodeBrowserForward = 0xA7,
    SNKeyCodeBrowserRefresh = 0xA8,
    SNKeyCodeBrowserStop = 0xA9,
    SNKeyCodeBrowserSearch = 0xAA,
    SNKeyCodeBrowserFavorites = 0xAB,
    SNKeyCodeBrowserHome = 0xAC,
    SNKeyCodeVolumeMute = 0xAD,
    SNKeyCodeVolumeDown = 0xAE,
    SNKeyCodeVolumeUp = 0xAF,
    SNKeyCodeMediaNextTrack = 0xB0,
    SNKeyCodeMediaPrevTrack = 0xB1,
    SNKeyCodeMediaStop = 0xB2,
    SNKeyCodeMediaPlayPause = 0xB3,
    SNKeyCodeLaunchMail = 0xB4,
    SNKeyCodeLaunchMediaSelect = 0xB5,
    SNKeyCodeLaunchApp1 = 0xB6,
    SNKeyCodeLaunchApp2 = 0xB7,
    SNKeyCodeReservedB8 = 0xB8,
    SNKeyCodeReservedB9 = 0xB9,
    SNKeyCodeSemicolon = 0xBA,
    SNKeyCodeEqual = 0xBB,
    SNKeyCodeComma = 0xBC,
    SNKeyCodeMinus = 0xBD,
    SNKeyCodePeriod = 0xBE,
    SNKeyCodeSlash = 0xBF,
    SNKeyCodeBackquote = 0xC0,
    SNKeyCodeReservedC1 = 0xC1,
    SNKeyCodeReservedC2 = 0xC2,
    SNKeyCodeReservedC3 = 0xC3,
    SNKeyCodeReservedC4 = 0xC4,
    SNKeyCodeReservedC5 = 0xC5,
    SNKeyCodeReservedC6 = 0xC6,
    SNKeyCodeReservedC7 = 0xC7,
    SNKeyCodeReservedC8 = 0xC8,
    SNKeyCodeReservedC9 = 0xC9,
    SNKeyCodeReservedCA = 0xCA,
    SNKeyCodeReservedCB = 0xCB,
    SNKeyCodeReservedCC = 0xCC,
    SNKeyCodeReservedCD = 0xCD,
    SNKeyCodeReservedCE = 0xCE,
    SNKeyCodeReservedCF = 0xCF,
    SNKeyCodeBracketLeft = 0xDB,
    SNKeyCodeBackslash = 0xDC,
    SNKeyCodeBracketRight = 0xDD,
    SNKeyCodeApostrophe = 0xDE,
    SNKeyCodeOem8 = 0xDF,
    SNKeyCodeReservedE0 = 0xE0,
    SNKeyCodeReservedE1 = 0xE1,
    SNKeyCodeOemAx = 0xE2,
    SNKeyCodeOem102 = 0xE3,
    SNKeyCodeIcoHelp = 0xE4,
    SNKeyCodeIco00 = 0xE5,
    SNKeyCodeProcesskey = 0xE6,
    SNKeyCodeIcoClear = 0xE7,
    SNKeyCodePacket = 0xE8,
    SNKeyCodeOemReset = 0xE9,
    SNKeyCodeOemJump = 0xEA,
    SNKeyCodeOemPa1 = 0xEB,
    SNKeyCodeOemPa2 = 0xEC,
    SNKeyCodeOemPa3 = 0xED,
    SNKeyCodeOemWsctrl = 0xEE,
    SNKeyCodeOemCusel = 0xEF,
    SNKeyCodeOemAttn = 0xF0,
    SNKeyCodeOemFinish = 0xF1,
    SNKeyCodeOemCopy = 0xF2,
    SNKeyCodeOemAuto = 0xF3,
    SNKeyCodeOemEnlw = 0xF4,
    SNKeyCodeOemBacktab = 0xF5,
    SNKeyCodeAttn = 0xF6,
    SNKeyCodeCrsel = 0xF7,
    SNKeyCodeExsel = 0xF8,
    SNKeyCodeEreof = 0xF9,
    SNKeyCodePlay = 0xFA,
    SNKeyCodeZoom = 0xFB,
    SNKeyCodeNoname = 0xFC,
    SNKeyCodePa1 = 0xFD,
    SNKeyCodeOemClear = 0xFE,
    SNKeyCodeReservedFF = 0xFF,
    SNKeyCodeNum
};

// マウスボタン
enum SNMouseButton
{
    SNMouseButtonDecide = 0,
    SNMouseButtonCancel,
    SNMouseButtonMiddle,
    SNMouseButtonWheelUp,
    SNMouseButtonWheelDown,
    SNMouseButtonNum,
};

// ゲームパッドID
enum SNGamePadID
{
    SNGamePadIDNull = 0xFF,
    SNGamePadID1 = 0,
    SNGamePadID2 = 1,
    SNGamePadIDNum
};

// ゲームパッドボタン
enum SNGamePadButton
{
    SNGamePadButtonNull = 0xFF,

    // ボタン定義
    SNGamePadButtonTop = 0,
    SNGamePadButton1 = SNGamePadButtonTop,
    SNGamePadButton2,
    SNGamePadButton3,
    SNGamePadButton4,
    SNGamePadButton5,
    SNGamePadButton6,
    SNGamePadButton7,
    SNGamePadButton8,
    SNGamePadButton9,
    SNGamePadButton10,
    SNGamePadButton11,
    SNGamePadButton12,
    SNGamePadButton13,
    SNGamePadButton14,
    SNGamePadButton15,
    SNGamePadButton16,
    SNGamePadButton17,
    SNGamePadButton18,
    SNGamePadButton19,
    SNGamePadButton20,
    SNGamePadButton21,
    SNGamePadButton22,
    SNGamePadButton23,
    SNGamePadButton24,
    SNGamePadButton25,
    SNGamePadButton26,
    SNGamePadButton27,
    SNGamePadButton28,
    SNGamePadButton29,
    SNGamePadButton30,
    SNGamePadButton31,
    SNGamePadButton32,
    // 軸定義
    SNGamePadAxisTop,
    SNGamePadAxis1Up = SNGamePadAxisTop,
    SNGamePadAxis1Down,
    SNGamePadAxis2Up,
    SNGamePadAxis2Down,
    SNGamePadAxis3Up,
    SNGamePadAxis3Down,
    SNGamePadAxis4Up,
    SNGamePadAxis4Down,
    SNGamePadAxis5Up,
    SNGamePadAxis5Down,
    SNGamePadAxis6Up,
    SNGamePadAxis6Down,
    SNGamePadAxis7Up,
    SNGamePadAxis7Down,
    // POV定義
    SNGamePadPOVTop,
    SNGamePadPOVUp = SNGamePadPOVTop,
    SNGamePadPOVDown,
    SNGamePadPOVLeft,
    SNGamePadPOVRight,
    // 合計ボタン数
    SNGamePadButtonNum
};



///////////////////////////////////////////////////////////////
// 仮想入力デバイス関連

// 仮想入力デバイス種別
enum SNInputVirtualDeviceType
{
    SNInputVirtualTypeGamePad1,         // 仮想ゲームパッド1
    SNInputVirtualTypeGamePad2,         // 仮想ゲームパッド2
    SNInputVirtualTypePointing,         // 仮想ポインティングデバイス
    SNInputVirtualTypeDirectKeyboard,   // 仮想ダイレクトキーボード
    SNInputVirtualTypeDirectGamePad,    // 仮想ダイレクトゲームパッド
    SNInputVirtualTypeNum
};

// 仮想ゲームパッドID
enum SNVirtualGamePadID
{
    SNVirtualGamePadIDTop,
    SNVirtualGamePadID1 = SNVirtualGamePadIDTop,
    SNVirtualGamePadID2,
    SNVirtualGamePadIDNum
};

// 仮想ゲームパッドボタン
enum SNVirtualGamePadButton
{
    SNVirtualGamePadNull = 0xFF,
    SNVirtualGamePadTop = 0,
    SNVirtualGamePadUp = SNVirtualGamePadTop,    // 方向キー上
    SNVirtualGamePadDown,      // 方向キー下
    SNVirtualGamePadLeft,      // 方向キー左
    SNVirtualGamePadRight,     // 方向キー右
    SNVirtualGamePadDecide,    // 決定(Aボタン)
    SNVirtualGamePadCancel,    // キャンセル(Bボタン)
    SNVirtualGamePadMenu,      // メニュー(Xボタン)
    SNVirtualGamePadAction,    // アクション(Yボタン)
    SNVirtualGamePadStart,     // スタート
    SNVirtualGamePadSelect,    // セレクト
    SNVirtualGamePadPagePrev,  // ページ戻し(Lボタン)
    SNVirtualGamePadPageNext,  // ページ送り(Rボタン)
    SNVirtualGamePadTriggerL,  // Lトリガ(L2ボタン)
    SNVirtualGamePadTriggerR,  // Rトリガ(R2ボタン)
    SNVirtualGamePadListUp,    // リストアップ(拡大)
    SNVirtualGamePadListDown,  // リストダウン(縮小)
    SNVirtualGamePadButtonNum
};

// 仮想ゲームパッドイベント
enum SNVirtualGamePadEvent
{
    SNVirtualGamePadEventTop,
    SNVirtualGamePadEventPush = SNVirtualGamePadEventTop,   // 押下
    SNVirtualGamePadEventPress,                             // 押下中
    SNVirtualGamePadEventLongPress,                         // 長押し
    SNVirtualGamePadEventRepeat,                            // リピート
    SNVirtualGamePadEventRelease,                           // 放す
    SNVirtualGamePadEventNum
};

// 仮想ポインティングイベント
enum SNVirtualPointingEvent
{
    SNVirtualPointingEventTop,
    SNVirtualPointingEventDecide = SNVirtualPointingEventTop,   // 決定
    SNVirtualPointingEventCancel,       // キャンセル
    SNVirtualPointingEventWheelUp,      // ホイールUP
    SNVirtualPointingEventWheelDown,    // ホイールDown
    SNVirtualPointingEventMove,         // 移動
    SNVirtualPointingEventDrag,         // ドラッグ
    SNVirtualPointingEventDrop,         // ドロップ
    SNVirtualPointingEventFlick,        // フリック
    SNVirtualPointingEventExBtn,        // 拡張ボタン
    SNVirtualPointingEventNum
};

// 仮想ポインティング情報
enum SNVirtualPointingInfo
{
    SNVirtualPointingInfoTop,
    SNVirtualPointingInfoCurrent = SNVirtualPointingInfoTop, // 現在位置
    SNVirtualPointingInfoPrevious,     // 前回位置
    SNVirtualPointingInfoDelta,        // 移動量
    SNVirtualPointingInfoDragStart,    // ドラッグ開始点
    SNVirtualPointingInfoFlickVelocity,// フリック速度
    SNVirtualPointingInfoNum
};


///////////////////////////////////////////////////////////////
// ストレージ関連

// ファイル処理結果
enum SNStorageResult
{
    SNStorageResultIdle,
    SNStorageResultProcessing,
    SNStorageResultNormal,
    SNStorageResultError,
    SNStorageResultExists,
    SNStorageResultNoExists,
};

// ファイル命令
enum SNStorageOperation
{
    SNStorageOperationNo,
    SNStorageOperationCheckExists,
    SNStorageOperationCreateFolder,
    SNStorageOperationRemoveFolder,
    SNStorageOperationFolderFileList,
    SNStorageOperationGetSize,
    SNStorageOperationWriteFile,
    SNStorageOperationReadFile,
    SNStorageOperationRemoveFile,
};


///////////////////////////////////////////////////////////////
// グラフィクス関連

// ビットマップフォントコードページ
enum SNBMCharPage
{
    BMPageSystem,   // システムフォント
    BMPageKanjiA,   // ア行
    BMPageKanjiK,   // カ行
    BMPageKanjiS,   // サ行
    BMPageKanjiT,   // タ行
    BMPageKanjiN,   // ナ行
    BMPageKanjiH,   // ハ行
    BMPageKanjiM,   // マ行
    BMPageKanjiY,   // ヤ行
    BMPageKanjiR,   // ラ行
    BMPageKanjiW,   // ワ行
    BMPageNum
};

// スケーリング
enum SNScalingType
{
    ScalingLetterBox,
    ScalingFit,
    ScalingFitH,
    ScalingFitV,
    ScalingTypeNum,
};


///////////////////////////////////////////////////////////////
// コーデック関連



// コーデック処理結果
enum SNImageCodecResult
{
    SNImageCodecResultIdle,
    SNImageCodecResultProcessing,
    SNImageCodecResultNormal,
    SNImageCodecResultError,
};

// コーデック命令
enum SNImageCodecOperation
{
    SNImageCodecOperationNo,
    SNImageCodecOperationDecode,
    SNImageCodecOperationEncode,
};



///////////////////////////////////////////////////////////////
// グラフィックス関連

// システムカラー定義
enum SNColorCode
{
    SNColorBlack,
    SNColorGray,
    SNColorWhite,
    SNColorDarkRed,
    SNColorDrakGreen,
    SNColorDarkBlue,
    SNColorDarkYellow,
    SNColorDarkPink,
    SNColorDarkAqua,
    SNColorRed,
    SNColorGreen,
    SNColorBlue,
    SNColorYellow,
    SNColorPink,
    SNColorAqua,
    SNColorNum
};

// フェードタイプ
enum SNGUIFadeType
{
    SNGUIFadeTypeCloss,
    SNGUIFadeTypeFromLeft,
    SNGUIFadeTypeFromRight,
    SNGUIFadeTypeFromTop,
    SNGUIFadeTypeFromBottom,
    SNGUIFadeTypeFromSide,
    SNGUIFadeTypeCenterToSide,
    SNGUIFadeTypeFromTopBottom,
    SNGUIFadeTypeCenterToTopBottom,
};

// フェード状態
enum SNGUIFadeState
{
    SNGUIFadeStateIdle,
    SNGUIFadeStateIn,
    SNGUIFadeStateOut,
    SNGUIFadeStateComplete
};
