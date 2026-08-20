#pragma once
#include "SNTypes.h"


//////////////////////////////////////////////////////
// グラフィクス関連

// フェードステップ
constexpr UInt8 SNFadeStepMin = 1;
constexpr UInt8 SNFadeStepMax = 255;

// アルファ値
constexpr UInt8 SNAlphaMin = 0;
constexpr UInt8 SNAlphaMax = 255;



//////////////////////////////////////////////////////
// サウンド関連

// 音量
constexpr UInt8 SNSoundVolMin = 0;
constexpr UInt8 SNSoundVolMax = 100;



//////////////////////////////////////////////////////
// ワールド関連

// カメラスケール
constexpr Float32 SNWViewScaleMin = 0.25f;
constexpr Float32 SNWViewScaleMax = 8.0f;
constexpr Float32 SNWViewScaleNormal = 1.0f;
constexpr Float32 SNWViewScaleStepLow = 0.25f;
constexpr Float32 SNWViewScaleStepHigh = 0.50f;


// 座標定義
constexpr UInt32 SNWGroundMeshSizeX = 256;
constexpr UInt32 SNWGroundMeshSizeY = 256;
constexpr UInt32 SNWGroundMeshSizeZ = 64;

constexpr Float32 SNWMeshLoadThresholdX = SNWGroundMeshSizeX * 0.1f;
constexpr Float32 SNWMeshLoadThresholdY = SNWGroundMeshSizeY * 0.1f;
constexpr Float32 SNWMeshLoadThresholdZ = SNWGroundMeshSizeZ * 0.1f;

constexpr Float32 SNWMeshPosXMin = 0.0f * SNWGroundMeshSizeX;
constexpr Float32 SNWMeshPosXMax = 1.0f * SNWGroundMeshSizeX;

constexpr Float32 SNWMeshPosYMin = 0.0f * SNWGroundMeshSizeY;
constexpr Float32 SNWMeshPosYMax = 1.0f * SNWGroundMeshSizeY;

constexpr Float32 SNWMeshPosZMin = 0.0f * SNWGroundMeshSizeZ;
constexpr Float32 SNWMeshPosZMax = 1.0f * SNWGroundMeshSizeZ;


 