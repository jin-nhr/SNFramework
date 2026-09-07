#pragma once
#include "SNFrameworkInternal.h"
#include "SNWObjectchip.h"
#include "SNSoftTimer.h"
#include "SNList.h"

struct SNWObjectEventParam
{
    UInt8 Event;
    UInt8 Param1;
    UInt8 Param2;
    UInt8 Param3;
    UInt32 Param32;
};

struct SNWObjectInfo
{
	SNWObjectchip::WObjectCode Code;	// オブジェクトコード
    UInt32 AnimationStep;               // アニメステップ

    Boolean Enable;                     // 有効
    Boolean Visible;                    // 可視
    Boolean Weight;                     // 重力影響

    SNWObjectState State;               // 状態
    SNWObjectState PrevState;           // 前状態

	SNWorldDir FrontDir;	        	// 向き
	SNWorldPos Pos;	        			// 座標
	SNWorldPos Acceleration;	        // 加速度

	SNWorldPos Speed;			        // 速度
};


class SNWObjectBase
{
public:
    static constexpr Int32 EventMaxNum = 8;
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

    // イベント通知
    virtual Void Notify(SNWObjectEventParam* param);

    // 更新
    virtual Void Update();

private:
    // イベント処理
    virtual Void EventFunc(SNWObjectEventParam* param);
    virtual Void EventFuncStop(SNWObjectEventParam* param);
    virtual Void EventFuncWalk(SNWObjectEventParam* param);
    virtual Void EventFuncJog(SNWObjectEventParam* param);
    virtual Void EventFuncJump(SNWObjectEventParam* param);

    // 状態更新
    virtual Void UpdateState();

    // アニメーション更新
    virtual Void UpdateAnimation();

    // 速度更新
    virtual Void UpdateSpeed();


    SNWObjectInfo Info;
    SNSoftTimer Timer;

    SNList EventList;
};

