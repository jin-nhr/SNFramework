#pragma once

// 基本データ型の定義

typedef void				Void;

typedef char				Int8;
typedef short				Int16;
typedef long				Int32;
typedef long long			Int64;
typedef int					Int;

typedef	unsigned char		UInt8;
typedef unsigned short		UInt16;
typedef unsigned long		UInt32;
typedef unsigned long long	UInt64;
typedef unsigned int		UInt;

typedef float				Float32;
typedef double				Float64;
typedef long double			Float128;

typedef unsigned char		Boolean;

typedef wchar_t				Char;
typedef wchar_t*			String;

typedef void*				Handle;



// 定数定義 コンパイル設定
static const Int SNApplicationIDLength = (260 + 4);		// アプリケーションIDの長さ
static const Int SNApplicationNameLength = (260 + 4);	// アプリケーション名の長さ
static const UInt8 StateNumMax = 8;						// 状態管理クラスが持つ最大状態数

// 定数定義 設定値
static const UInt8 SurfaceTypeDDB = 0;					// サーフェスタイプDDB
static const UInt8 SurfaceTypeDIB = 1;					// サーフェスタイプDIB



// 座標データ
struct SNPoint
{
	Int32 X;		// X座標
	Int32 Y;		// Y座標
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
