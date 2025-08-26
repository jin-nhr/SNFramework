#pragma once

// 構造体定義

// 座標データ
struct SNPoint
{
	Int32 X;		// X座標
	Int32 Y;		// Y座標
};

// サイズデータ
struct SNSize
{
    Int32 Width;    // 幅
    Int32 Height;   // 高さ
};

// 矩形データ
struct SNRect
{
	Int32 PointX;	// X座標
	Int32 PointY;	// Y座標
	Int32 Width;	// 幅
	Int32 Height;	// 高さ
};

// 色データ
struct SNColor
{
	UInt8 Blue;		// 青成分
	UInt8 Green;	// 緑成分
	UInt8 Red;		// 赤成分
	UInt8 Alpha;	// アルファ値
};

// ビットマップ情報
struct SNBitmapInfo
{
	UInt32 StructureSize;	// 構造体サイズ = 40
	Int32 Width;			// ビットマップの幅
	Int32 Height;			// ビットマップの高さ
	UInt16 Planes;			// ビットマップのプレーン数 = 1
	UInt16 BitCount;		// 1ピクセルあたりのビット数 = 32
	UInt32 Compression;		// 圧縮状態 = BI_RGB
	UInt32 ImageSize;		// ピクセルデータサイズ(バイト単位) = 0
	Int32 XPerMeter;		// X方向の解像度 = 0
	Int32 YPerMeter;		// Y方向の解像度 = 0
	UInt32 ColorUsed;		// 使用色数 = 0
	UInt32 ColorImportant;	// 重要色数 = 0
	UInt8 Reserve[4];		// リザーブ
};


// 入力ボタンイベント
struct SNInputButtonEvent
{
    Boolean Push;           // 押下
    Boolean Press;          // 押下中
    Boolean LongPress;      // 長押し
    Boolean Repeat;         // リピート
    Boolean Release;        // 放す
};

// 入力ポインティングイベント
struct SNInputPointingEvent
{
    Boolean Decide;         // 決定
    Boolean Cancel;         // キャンセル
    Boolean WheelUp;        // ホイールUp
    Boolean WheelDown;      // ホイールDown
    Boolean Move;           // 移動
    Boolean Drag;           // ドラッグ
    Boolean Drop;           // ドロップ
    Boolean Frick;          // フリック
};

// 入力ポインティング情報
struct SNInputPointingInfo
{
    SNPoint CurrentPosition;        // 現在座標
    SNPoint PreviousPosition;       // 前回座標
    SNPoint DeltaPosition;          // 差分
    SNPoint DragStartPosition;      // ドラッグ開始点
    SNPoint FrickVelocity;          // フリック速度
};

// 入力イベント
struct SNInputDeviceEvent
{
    SNInputButtonEvent ButtonEvent[SNInputDeviceNum][SNInputButtonNum]; // ボタンイベント
    SNInputPointingEvent PointingEvent;               // ポインティングイベント
    SNInputPointingInfo PointingInfo;                 // ポインティング情報
};

// 物理入力イベント
struct SNPhysicalInputEvent
{
    const Boolean* Keyboard;                  // キーボード
    const Boolean* MouseButton;               // マウスボタン
    const SNPoint* MousePosition;             // マウス座標
    const Boolean* GamePad[SNGamePadIDNum];   // ゲームパッド
};

// 入力デバイスマッピング
struct SNInputMapping
{
    SNKeyCode KeyboadMapping[SNInputButtonNum];     // キーボードマッピング
    SNMouseButton MouseMapping[SNInputButtonNum];   // マウスボタンマッピング
    SNGamePadButton GamePadMapping[SNGamePadIDNum][SNInputButtonNum];   // ゲームパッドマッピング
};

// アプリケーションイベント情報
struct SNApplicationEventInfo
{
    Boolean Active;			    // アクティブ通知
    Boolean NonActive;		    // 非アクティブ通知
    Boolean ExitApplication;	// 終了通知
    Boolean WheelUp;			// ホイールUp通知
    Boolean WheelDown;		    // ホイールDown通知
};

// ループバックイベント
struct SNLoopbackEvent
{
    UInt32 Reserve;
};

// イベント処理リザルト
struct SNEventResult
{
    Boolean ExitApplication;
    UInt8 Reserve[3];
};
