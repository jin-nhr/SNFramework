#pragma once
#include "SNTypes.h"
#include "SNEnum.h"



///////////////////////////////////////////////////////////////
// グラフィック関連

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
    Int32 Width;	    // 幅
    Int32 Height;	// 高さ
};

// 色データ
struct SNColor
{
    UInt8 Blue;		// 青成分
    UInt8 Green;	    // 緑成分
    UInt8 Red;		// 赤成分
    UInt8 Alpha;	    // アルファ値
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
