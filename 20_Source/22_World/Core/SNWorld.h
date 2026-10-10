#pragma once
#include "SNFrameworkInternal.h"
#include "SNWNearbySpace.h"
#include "SNWMeshManager.h"
#include "SNWActObject.h"
#include "SNWPhysics.h"
#include "SNWorkerThread.h"
#include "SNCriticalSection.h"

// ワールドクラス


struct SNGroundEffectInfo
{
	SNWorldPos Pos;
	UInt64 Effect;
};


class SNWorld
{
public:
	static constexpr SNWorldDir TimeToGlobalLight[SNWTimeStepNum] =
	{
		SNWorldDirN,
		SNWorldDirN,
		SNWorldDirN,
		SNWorldDirN,
		SNWorldDirE,
		SNWorldDirE,
		SNWorldDirE,
		SNWorldDirE,
		SNWorldDirE,
		SNWorldDirE,
		SNWorldDirS,
		SNWorldDirS,
		SNWorldDirS,
		SNWorldDirS,
		SNWorldDirS,
		SNWorldDirS,
		SNWorldDirW,
		SNWorldDirW,
		SNWorldDirW,
		SNWorldDirW,
		SNWorldDirW,
		SNWorldDirW,
		SNWorldDirN,
		SNWorldDirN,
	};

	static constexpr SNWTimeZone TimeToTimeZone[SNWTimeStepNum] =
	{
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneMorning,
		SNWTimeZoneMorning,
		SNWTimeZoneMorning,
		SNWTimeZoneMorning,
		SNWTimeZoneMorning,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneAfternoon,
		SNWTimeZoneEvening,
		SNWTimeZoneEvening,
		SNWTimeZoneEvening,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
		SNWTimeZoneNight,
	};

	// 方位はNE固定のため左に45°傾く
	static constexpr Int8 DirToAngleTable[SNWorldDirNum] =
	{
		0,			// Top
		95,			// 上
		63,			// 右上
		31,			// 右
		0,			// 右下
		-31,		// 下
		-63,		// 左下
		-95,		// 左
		127,		// 左上
	};

	static constexpr Int8 ParallelProcNum = SNSystemConfig::ParallelProcMax;
	static constexpr Int8 WorkerThreadNum = ParallelProcNum - 1;
	static constexpr Int8 WorkerInfoNum = 2;

	static constexpr Int64 GroundEffectInfoMax = (SNWNearbyObjectNum / (WorkerThreadNum + 1)) + 1;

public:
	// 初期化
	static Void Initialize();

	// 終了
	static Void Terminate();

	// 開始
	static Void Start();

	// 終了
	static Void End();

	// 一時停止
	static Void Pause();

	// 更新
	static Void Update();


	///////////////////////////////////////////////////////////////////
	// ワールド操作系

	// PC取得
	static SNWActObject* GetPCObject();

	// カレント座標設定
	static Void SetCurrentPos(SNWorldPos* pos);

	// カレント移動
	static Void MoveCurrentPos(SNWorldPos* pos);

	// 地形書き込み
	static Void WriteGroundData(SNWorldPos* pos, SNMapchip::SNMapchipCode code);

	static Void FlushGroundData();

	// オブジェクトリスト取得
	static SNWNearbySpace* GetNearbySpace();

	// グローバルオブジェクト取得
	static SNWGlobalObject* GetGlobalObject();

	// アニメステップ取得
	static Int32 GetAGroundAnimeStep();

	// 時間取得
	static SNWTimeZone GetTimeZone();

	// 方向→角度変換
	static Int8 DirToAngle(SNWorldDir dir);

protected:
	// エフェクト登録
	static Void RegisterNearbyEffect();

	static Void RegisterGroundEffect();

	static Void RegisterGroundEffectImp(UInt32 id, Void* param);

	// 地形エフェクト処理
	static Void RegisterNearbyEffectGround(UInt32 id, SNWNearbyObject* obj_ptr);

	// 地形の投影チェック
	static Boolean JudgeGroundPShadow(Int32 x, Int32 y, Int32 z, Boolean* lt, Boolean* rt, Boolean* lb, Boolean* rb);

	// ワールドタイム更新
	static Void UpdateWorldTime();


private:
	static Boolean Run;
	static Boolean Suspend;
	static SNWorldPos CurrentPos;	// 現在座標
	static UInt32 WorldCount;		// ワールドカウンタ
	static UInt32 WorldTime;		// ワールド時間
	static SNSoftTimer GroundAnimeTimer;
	static Int32 GroundAnimeStep;

	static SNWMeshManager MeshManager;		// メッシュ管理

	static SNWGlobalObject GlobalObject;
	
	static SNWNearbySpace NearbySpace;	// 周辺空間

	static Int32 TimeHour;	// j時間
	static SNWorldDir GlobalLight;	// グローバル光源の位置

	static SNWActObject PCObject;

	static SNWPhysics Physics;

	static SNWorkerThread Worker[WorkerThreadNum];
	static Int64 WorkerInfo[ParallelProcNum][WorkerInfoNum];
};
