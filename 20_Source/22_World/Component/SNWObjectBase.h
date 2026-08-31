#pragma once
#include "SNFrameworkInternal.h"
#include "SNWObjectchip.h"


struct SNWObjectInfo
{
	SNWObjectchip::WObjectCode Code;	// オブジェクトコード
    UInt32 AnimationStep;               // アニメステップ
    UInt32 AnimationCount;              // アニメカウント

    Boolean Enable;                     // 有効
    Boolean Visible;                    // 可視
    Boolean Weight;                     // 重力影響

    SNWObjectState State;               // 状態

	SNWorldDir FrontDir;	        	// 向き
	SNWorldPos Pos;	        			// 座標
	SNWorldPos Accelaration;	        // 加速度

	SNWorldPos Speed;			        // 速度
};


class SNWObjectBase
{
public:
    SNWObjectBase();

    virtual ~SNWObjectBase();

    virtual Void Initialize();

    virtual Void Terminate();

    // ObjectInfo参照
    virtual SNWObjectInfo* RefInfo();

    // コード取得
    virtual UInt16 GetCode();

    // アニメステート取得
    virtual SNWObjectchip::SNWActState GetActState();

    // 方向計算
    virtual SNWorldDir CalcDir(SNWorldDir base_dir);

    virtual Void InitObjectInfo();

private:
    SNWObjectInfo Info;
};

