#pragma once
#include "SNFrameworkInternal.h"
#include "SNBitmap.h"
#include "SNWGround.h"
#include "SNMapchip.h"
#include "SNList.h"

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

	// 影方向計算
	static SNWorldShadowDir CalcShadowDir(SNWorldDir dir);


	///////////////////////////////////////////////////////////////////
	// ワールド自動実行系

	// :

private:
	static Boolean Run;

	static SNWorldPos CurrentPos;

	static SNWGround Ground;
};
