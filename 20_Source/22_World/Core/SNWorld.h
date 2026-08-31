#pragma once
#include "SNFrameworkInternal.h"
#include "SNWNearbySpace.h"
#include "SNWMeshManager.h"

// ワールドクラス



class SNWorld
{
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

	// カレント座標設定
	static Void SetCurrentPos(SNWorldPos* pos);

	// カレント移動
	static Void MoveCurrentPos(SNWorldPos* pos);

	// 地形書き込み
	static Void WriteGroundData(SNMapchip::SNMapchipCode code);

	// オブジェクトリスト取得
	static SNWNearbySpace* GetNearbySpace();

	static SNWEasyLightDir RefEasyLightDir();

protected:
	// 地形更新
	static Void UpdateGround();

	// オブジェクト更新
	static Void UpdateGlobalObject();

	// エフェクト登録
	static Void RegisterNearbyEffect();

	// 地形エフェクト処理
	static Void RegisterNearbyEffectGround(SNWNearbyObject* obj_ptr);

	// 地形の投影チェック
	static Boolean JudgeGroundPShadow(Int32 x, Int32 y, Int32 z);


private:
	static Boolean Run;
	static Boolean Suspend;
	static SNWorldPos CurrentPos;	// 現在座標
	static UInt32 WorldTime;		// ワールド時間

	static SNWMeshManager MeshManager;		// メッシュ管理

	static SNWGlobalObject GlobalObject;
	
	static SNWNearbySpace NearbySpace;	// 周辺空間

	static SNWorldDir GlobalLight;	// グローバル光源の方向(光の進む方向)
	static SNWEasyLightDir EasyLight;	// 簡易光源方向

};
