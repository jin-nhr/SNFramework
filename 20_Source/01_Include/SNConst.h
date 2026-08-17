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
constexpr Float32 SNWOCameraScaleMin = 0.1f;
constexpr Float32 SNWOCameraScaleMax = 8.0f;

// 座標定義
constexpr UInt32 SNWOGroundMeshSizeX = 256;
constexpr UInt32 SNWOGroundMeshSizeY = 256;
constexpr UInt32 SNWOGroundMeshSizeZ = 64;

constexpr Float32 SNWOMeshPosXMin = 0.0f * SNWOGroundMeshSizeX;
constexpr Float32 SNWOMeshPosXMax = 1.0f * SNWOGroundMeshSizeX;

constexpr Float32 SNWOMeshPosYMin = 0.0f * SNWOGroundMeshSizeY;
constexpr Float32 SNWOMeshPosYMax = 1.0f * SNWOGroundMeshSizeY;

constexpr Float32 SNWOMeshPosZMin = 0.0f * SNWOGroundMeshSizeZ;
constexpr Float32 SNWOMeshPosZMax = 1.0f * SNWOGroundMeshSizeZ;


 