#include "SNWorld.h"
#include "SNSystemConfig.h"
#include "SNWGlobalObject.h"
#include "SNMath.h"
#include "SNWindowsAPI.h"

SNWorldPos SNWorld::CurrentPos = {0};
SNWMeshManager SNWorld::MeshManager;
SNWGlobalObject SNWorld::GlobalObject;
UInt32 SNWorld::WorldCount = 0; 
UInt32 SNWorld::WorldTime = 0;

SNSoftTimer SNWorld::GroundAnimeTimer;
Int32 SNWorld::GroundAnimeStep;

Boolean SNWorld::Run = false;
Boolean SNWorld::Suspend = false;
SNWNearbySpace SNWorld::NearbySpace;	// 周辺空間

Int32 SNWorld::TimeHour;
SNWorldDir SNWorld::GlobalLight;

SNWActObject SNWorld::PCObject;

SNWPhysics SNWorld::Physics;

SNWorkerThread SNWorld::Worker[WorkerThreadNum];
Int64 SNWorld::WorkerInfo[WorkerThreadNum + 1][WorkerInfoNum];


// 初期化
Void SNWorld::Initialize()
{
	Int32 cnt;
	Int32 cnt2;

	CurrentPos = {0};
	WorldCount = 0;
	WorldTime = 0;
	Run = false;
	Suspend = false;

	TimeHour = 0;
	GlobalLight = TimeToGlobalLight[TimeHour];

	MeshManager.Initialize();
	NearbySpace.Initialize();

	GlobalObject.Initialize();
	GlobalObject.Load();

	// PCオブジェクトの設定
	PCObject.SetObject(GlobalObject.RefObject(SNWGlobalObjectPlayer));

	GroundAnimeTimer.Initialize();
	GroundAnimeStep = 0;

	// 物理エンジンの初期化
	Physics.Initialize();

	// 物理エンジン設定
	Physics.SetSpace(&NearbySpace);
	Physics.SetGlobalObject(&GlobalObject);

	for (cnt = 0; cnt < WorkerThreadNum + 1; cnt++)
	{
		for (cnt2 = 0; cnt2 < WorkerInfoNum; cnt2++)
		{
			WorkerInfo[cnt][cnt2] = 0;
		}
	}

	return;
}

// 終了
Void SNWorld::Terminate()
{
	NearbySpace.Terminate();
	MeshManager.Terminate();

	GlobalObject.Save();
	GlobalObject.Terminate();

	// 物理エンジン終了
	Physics.Terminate();

	return;
}

Void SNWorld::Start()
{
	Run = true;
	Suspend = false;
	GroundAnimeTimer.Start(SNSystemConfig::GroundAnimeInterval);
	return;
}

Void SNWorld::End()
{
	Run = false;
	GroundAnimeTimer.Stop();
	return;
}

Void SNWorld::Pause()
{
	Suspend = true;
	GroundAnimeTimer.Stop();

	return;
}

// 更新
Void SNWorld::Update()
{
	if (Run && !Suspend)
	{
		// 更新準備
		WorldCount++;
		NearbySpace.UpdateStart(&CurrentPos, WorldCount);

		UpdateWorldTime();

		// 地形アニメカウンタ制御
		if (GroundAnimeTimer.IsTimeout())
		{
			GroundAnimeTimer.Restart();
			GroundAnimeStep = (Int32)SNMath::Increment(GroundAnimeStep, 0, SNMapchip::MapchipAnimeStep - 1);
		}

		// 地形更新
		MeshManager.Update(&CurrentPos);

		// グローバルオブジェクト更新
		GlobalObject.Update();

		// 地形オブジェクト登録
		MeshManager.RegisterNearbyObject(&NearbySpace);

		// グローバルオブジェクト登録
		GlobalObject.RegisterNearbyObject(&NearbySpace);

		// 物理エンジン実行
		Physics.Update();

		// 地形エフェクト登録
		RegisterGroundEffect();

		// グローバルオブジェクトの影登録
		GlobalObject.RegisterShadow(&NearbySpace);
	}

	return;
}


// PC取得
SNWActObject* SNWorld::GetPCObject()
{
	return &PCObject;
}

// カレント座標設定
Void SNWorld::SetCurrentPos(SNWorldPos* pos)
{
	CurrentPos = *pos;

	return;
}

// カレント座標移動
Void SNWorld::MoveCurrentPos(SNWorldPos* pos)
{
	CurrentPos.X += pos->X;
	CurrentPos.Y += pos->Y;
	CurrentPos.Z += pos->Z;

	return;
}

// 地形書き込み
Void SNWorld::WriteGroundData(SNWorldPos* pos, SNMapchip::SNMapchipCode code)
{
	MeshManager.Write(pos, code);

	return;
}

Void SNWorld::FlushGroundData()
{
	MeshManager.RunWrite();

	return;
}

SNWNearbySpace* SNWorld::GetNearbySpace()
{
	return &NearbySpace;
}

SNWGlobalObject* SNWorld::GetGlobalObject()
{
	return &GlobalObject;
}

Int32 SNWorld::GetAGroundAnimeStep()
{
	return GroundAnimeStep;
}

SNWTimeZone SNWorld::GetTimeZone()
{
	return TimeToTimeZone[TimeHour];
}

Int8 SNWorld::DirToAngle(SNWorldDir dir)
{
	return DirToAngleTable[dir];
}

// 周辺空間へのエフェクト登録
Void SNWorld::RegisterNearbyEffect()
{


	return;
}

Void SNWorld::RegisterGroundEffect()
{
	UInt32 obj_num = (UInt32)NearbySpace.GetObjectNum();
	UInt32 cnt;
	UInt32 start_step = (obj_num / ParallelProcNum) + (Int64)((obj_num % ParallelProcNum) != 0);
	UInt32 start = 0;
	Boolean run = true;

	for (cnt = 0; cnt < WorkerThreadNum; cnt++)
	{
		// ワーカー用の情報セット
		WorkerInfo[cnt][0] = start;
		WorkerInfo[cnt][1] = SNMath::SelectMin(obj_num, start_step);
		obj_num -= start_step;
		start += start_step;

		// 実行関数を登録/実行
		Worker[cnt].ID = cnt;
		Worker[cnt].WorkerFunc = RegisterGroundEffectImp;
		Worker[cnt].IsComplete = false;
		Worker[cnt].Run();
	}

	WorkerInfo[cnt][0] = start;
	WorkerInfo[cnt][1] = SNMath::SelectMin(obj_num, start_step);

	// 自分もワーカー処理
	RegisterGroundEffectImp(cnt, nullptr);

	// ワーカースレッドすべてが完了するまで待つ
	while (run)
	{
		Sleep(0);

		run = false;
		for (cnt = 0; cnt < WorkerThreadNum; cnt++)
		{
			run |= (!Worker[cnt].IsComplete);
		}
	}
}

// 周辺空間へのエフェクト登録
Void SNWorld::RegisterGroundEffectImp(UInt32 id, Void* param)
{
	SNWNearbyObject* obj_ptr = nullptr;
	Int64 cnt;

	// 登録されたオブジェクトを走査

	for (cnt = WorkerInfo[id][0]; cnt < WorkerInfo[id][0] + WorkerInfo[id][1]; cnt++)
	{
		obj_ptr = NearbySpace.RefObject((Int32)cnt);

		switch (obj_ptr->Type)
		{
		case SNWNearbyObjectTypeGround:
			RegisterNearbyEffectGround(id, obj_ptr);
			break;
		}
	}

	return;
}



Void SNWorld::RegisterNearbyEffectGround(UInt32 id, SNWNearbyObject* obj_ptr)
{
	Int32 x, y, z;
	UInt64 effect_flg = 0;
	Boolean exist_u;
	Boolean exist_r;
	Boolean exist_b;
	Boolean exist_l;
	Boolean exist_top;

	Boolean exist_uu;
	Boolean exist_ru;
	Boolean exist_bu;
	Boolean exist_lu;

	Boolean exist_bottom;

	Boolean ps_lt;
	Boolean ps_lb;
	Boolean ps_rt;
	Boolean ps_rb;


	static UInt64 gshadow[SNWorldDirNum] =
	{
		SNWNearbyEffectGroundBitGShadowB,
		SNWNearbyEffectGroundBitGShadowB,
		SNWNearbyEffectGroundBitGShadowB,
		SNWNearbyEffectGroundBitGShadowL,
		SNWNearbyEffectGroundBitGShadowL,
		SNWNearbyEffectGroundBitGShadowU,
		SNWNearbyEffectGroundBitGShadowU,
		SNWNearbyEffectGroundBitGShadowR,
		SNWNearbyEffectGroundBitGShadowR,
	};

	x = (Int32)obj_ptr->Pos.X;
	y = (Int32)obj_ptr->Pos.Y;
	z = (Int32)obj_ptr->Pos.Z;

	// 周囲4つが地形？
	exist_u = NearbySpace.IsBlocked(x, y - 1, z, SNWNearbyObjectTypeGround);
	exist_r = NearbySpace.IsBlocked(x + 1, y, z, SNWNearbyObjectTypeGround);
	exist_b = NearbySpace.IsBlocked(x, y + 1, z, SNWNearbyObjectTypeGround);
	exist_l = NearbySpace.IsBlocked(x - 1, y, z, SNWNearbyObjectTypeGround);

	exist_top = NearbySpace.IsBlocked(x, y, z + 1, SNWNearbyObjectTypeGround);

	exist_uu = NearbySpace.IsBlocked(x, y - 1, z + 1, SNWNearbyObjectTypeGround);
	exist_ru = NearbySpace.IsBlocked(x + 1, y, z + 1, SNWNearbyObjectTypeGround);
	exist_bu = NearbySpace.IsBlocked(x, y + 1, z + 1, SNWNearbyObjectTypeGround);
	exist_lu = NearbySpace.IsBlocked(x - 1, y, z + 1, SNWNearbyObjectTypeGround);

	exist_bottom = NearbySpace.IsBlocked(x, y, z - 1, SNWNearbyObjectTypeGround);

	// 周囲に地形以外のセルがある
	if (!(exist_u && exist_r && exist_b && exist_l))
	{
		// グローバル光源の反映
		effect_flg |= gshadow[GlobalLight];

		if (!exist_top)
		{
			// ボーダーの設定 周りがないとき
			effect_flg |= (exist_u ? 0 : SNWNearbyEffectGroundBitBorderU);
			effect_flg |= (exist_r ? 0 : SNWNearbyEffectGroundBitBorderR);
			effect_flg |= (exist_b ? 0 : SNWNearbyEffectGroundBitBorderB);
			effect_flg |= (exist_l ? 0 : SNWNearbyEffectGroundBitBorderL);
				}
	}

	// ボーダー追加設定 周りが高いとき
	if (!exist_top)
	{
		// ボーダーの反映
		effect_flg |= (exist_uu ? SNWNearbyEffectGroundBitBorderU : 0);
		effect_flg |= (exist_ru ? SNWNearbyEffectGroundBitBorderR : 0);
		effect_flg |= (exist_bu ? SNWNearbyEffectGroundBitBorderB : 0);
		effect_flg |= (exist_lu ? SNWNearbyEffectGroundBitBorderL : 0);
	}

	effect_flg |= (exist_u ? 0 : SNWNearbyEffectGroundBitBorderSideU);
	effect_flg |= (exist_r ? 0 : SNWNearbyEffectGroundBitBorderSideR);
	effect_flg |= (exist_b ? 0 : SNWNearbyEffectGroundBitBorderSideB);
	effect_flg |= (exist_l ? 0 : SNWNearbyEffectGroundBitBorderSideL);
	effect_flg |= (exist_bottom ? 0 : SNWNearbyEffectGroundBitBorderBottom);


	// 周囲と上に地形以外のセルがある
	if (!(exist_u && exist_r && exist_b && exist_l && exist_top))
	{
		ps_lt = false;
		ps_rt = false;
		ps_lb = false;
		ps_rb = false;

		// 影を作る地形があるか調べる
		if (JudgeGroundPShadow(x, y, z, &ps_lt, &ps_rt, &ps_lb, &ps_rb))
		{
			// 投影情報をセット
			effect_flg |= (exist_b || (GlobalLight != SNWorldDirS) ? 0 : SNWNearbyEffectGroundBitPShadowB);
			effect_flg |= (exist_b || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowB);
			effect_flg |= (exist_b || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowB);

			effect_flg |= (exist_l || (GlobalLight != SNWorldDirW) ? 0 : SNWNearbyEffectGroundBitPShadowL);
			effect_flg |= (exist_l || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowL);
			effect_flg |= (exist_l || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowL);

			effect_flg |= (exist_u || (GlobalLight != SNWorldDirN) ? 0 : SNWNearbyEffectGroundBitPShadowU);
			effect_flg |= (exist_u || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowU);
			effect_flg |= (exist_u || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowU);

			effect_flg |= (exist_r || (GlobalLight != SNWorldDirE) ? 0 : SNWNearbyEffectGroundBitPShadowR);
			effect_flg |= (exist_r || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowR);
			effect_flg |= (exist_r || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowR);

			effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowT);
		}

		else
		{
			if (ps_lt)
			{
				effect_flg |= (exist_l || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowL);
				effect_flg |= (exist_l || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowL);

				effect_flg |= (exist_u || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowU);
				effect_flg |= (exist_u || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowU);

				effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowTUL);
			}
			if (ps_rt)
			{
				effect_flg |= (exist_u || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowU);
				effect_flg |= (exist_u || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowU);

				effect_flg |= (exist_r || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowR);
				effect_flg |= (exist_r || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowR);

				effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowTUR);
			}
			if (ps_lb)
			{
				effect_flg |= (exist_b || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowB);
				effect_flg |= (exist_b || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowB);

				effect_flg |= (exist_l || (GlobalLight != SNWorldDirNW) ? 0 : SNWNearbyEffectGroundBitPShadowL);
				effect_flg |= (exist_l || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowL);

				effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowTBL);
			}
			if (ps_rb)
			{
				effect_flg |= (exist_b || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowB);
				effect_flg |= (exist_b || (GlobalLight != SNWorldDirSW) ? 0 : SNWNearbyEffectGroundBitPShadowB);

				effect_flg |= (exist_r || (GlobalLight != SNWorldDirNE) ? 0 : SNWNearbyEffectGroundBitPShadowR);
				effect_flg |= (exist_r || (GlobalLight != SNWorldDirSE) ? 0 : SNWNearbyEffectGroundBitPShadowR);

				effect_flg |= (exist_top ? 0 : SNWNearbyEffectGroundBitPShadowTBR);
			}
		}
	}

	// 周辺空間へエフェクト登録
	NearbySpace.RegisterGroundEffectToSpace(obj_ptr, effect_flg);

	return;
}

Boolean SNWorld::JudgeGroundPShadow(Int32 x, Int32 y, Int32 z, Boolean* lt, Boolean* rt, Boolean* lb, Boolean* rb)
{
	Boolean ret = false;
	Int32 step_x = 0;
	Int32 step_y = 0;

	Int32 ref_x = x;
	Int32 ref_y = y;
	Int32 ref_z = z;

	Int32 srch_cnt;

	Int32 near_shadow_u = false;
	Int32 near_shadow_r = false;
	Int32 near_shadow_b = false;
	Int32 near_shadow_l = false;

	*lt = false;
	*rt = false;
	*lb = false;
	*rb = false;

	switch (GlobalLight)
	{
	case SNWorldDirN:
		step_x = 0;
		step_y = -1;
		break;
	case SNWorldDirNE:
		step_x = 1;
		step_y = -1;
		break;
	case SNWorldDirE:
		step_x = 1;
		step_y = 0;
		break;
	case SNWorldDirSE:
		step_x = 1;
		step_y = 1;
		break;
	case SNWorldDirS:
		step_x = 0;
		step_y = 1;
		break;
	case SNWorldDirSW:
		step_x = -1;
		step_y = 1;
		break;
	case SNWorldDirW:
		step_x = -1;
		step_y = 0;
		break;
	case SNWorldDirNW:
		step_x = -1;
		step_y = -1;
		break;
	default:
		step_x = 0;
		step_y = -1;
		break;
	}

	// 対象オブジェクトが影投影対象化判定
	if ((SNSystemConfig::GroundPShadowOutRange < ref_x) &&
		(ref_x < SNWNearbySpaceSizeX - SNSystemConfig::GroundPShadowOutRange) &&
		(SNSystemConfig::GroundPShadowOutRange < ref_y) &&
		(ref_y < SNWNearbySpaceSizeY - SNSystemConfig::GroundPShadowOutRange) &&
		(SNSystemConfig::GroundPShadowOutRange < ref_z) &&
		(ref_z < SNWNearbySpaceSizeZ - SNSystemConfig::GroundPShadowOutRange))
	{
		ref_x += step_x;
		ref_y += step_y;
		ref_z += 1;

		srch_cnt = 0;

		while (NearbySpace.RefSpace(ref_x, ref_y, ref_z) &&
			   (SNSystemConfig::GroundPShadowSearchRange >= srch_cnt))
		{
			ret = false;

			switch (GlobalLight)
			{
			case SNWorldDirN:
			case SNWorldDirS:
			case SNWorldDirE:
			case SNWorldDirW:
				ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z, SNWNearbyObjectTypeGround);
				ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z + 1, SNWNearbyObjectTypeGround);
				ref_x += step_x;
				ref_y += step_y;
				ref_z += 2;
				break;
			case SNWorldDirNE:
			case SNWorldDirSE:
			case SNWorldDirSW:
			case SNWorldDirNW:
				ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z, SNWNearbyObjectTypeGround);
				ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z + 1, SNWNearbyObjectTypeGround);
				ret |= NearbySpace.IsBlocked(ref_x, ref_y, ref_z + 2, SNWNearbyObjectTypeGround);
				near_shadow_u |= NearbySpace.IsBlocked(ref_x, ref_y - 1, ref_z, SNWNearbyObjectTypeGround);
				near_shadow_u |= NearbySpace.IsBlocked(ref_x, ref_y - 1, ref_z + 1, SNWNearbyObjectTypeGround);
				near_shadow_u |= NearbySpace.IsBlocked(ref_x, ref_y - 1, ref_z + 2, SNWNearbyObjectTypeGround);
				near_shadow_r |= NearbySpace.IsBlocked(ref_x + 1, ref_y, ref_z, SNWNearbyObjectTypeGround);
				near_shadow_r |= NearbySpace.IsBlocked(ref_x + 1, ref_y, ref_z + 1, SNWNearbyObjectTypeGround);
				near_shadow_r |= NearbySpace.IsBlocked(ref_x + 1, ref_y, ref_z + 2, SNWNearbyObjectTypeGround);
				near_shadow_b |= NearbySpace.IsBlocked(ref_x, ref_y + 1, ref_z, SNWNearbyObjectTypeGround);
				near_shadow_b |= NearbySpace.IsBlocked(ref_x, ref_y + 1, ref_z + 1, SNWNearbyObjectTypeGround);
				near_shadow_b |= NearbySpace.IsBlocked(ref_x, ref_y + 1, ref_z + 2, SNWNearbyObjectTypeGround);
				near_shadow_l |= NearbySpace.IsBlocked(ref_x - 1, ref_y, ref_z, SNWNearbyObjectTypeGround);
				near_shadow_l |= NearbySpace.IsBlocked(ref_x - 1, ref_y, ref_z + 1, SNWNearbyObjectTypeGround);
				near_shadow_l |= NearbySpace.IsBlocked(ref_x - 1, ref_y, ref_z + 2, SNWNearbyObjectTypeGround);
				ref_x += step_x;
				ref_y += step_y;
				ref_z += 3;
				break;
			}

			if (ret)
			{
				break;
			}

			srch_cnt++;
		}
	}

	if (!ret)
	{
		switch (GlobalLight)
		{
		case SNWorldDirNE:
			near_shadow_u |= NearbySpace.IsBlocked(x, y - 1, z + 1, SNWNearbyObjectTypeGround);
			near_shadow_r |= NearbySpace.IsBlocked(x + 1, y, z + 1, SNWNearbyObjectTypeGround);
			*lt = near_shadow_l && near_shadow_u;
			*rb = near_shadow_r && near_shadow_b;
			break;
		case SNWorldDirSE:
			near_shadow_r |= NearbySpace.IsBlocked(x + 1, y, z + 1, SNWNearbyObjectTypeGround);
			near_shadow_b |= NearbySpace.IsBlocked(x, y + 1, z + 1, SNWNearbyObjectTypeGround);
			*rt = near_shadow_r && near_shadow_u;
			*lb = near_shadow_l && near_shadow_b;
			break;
		case SNWorldDirSW:
			near_shadow_b |= NearbySpace.IsBlocked(x, y + 1, z + 1, SNWNearbyObjectTypeGround);
			near_shadow_l |= NearbySpace.IsBlocked(x - 1, y, z + 1, SNWNearbyObjectTypeGround);
			*lt = near_shadow_l && near_shadow_u;
			*rb = near_shadow_r && near_shadow_b;
			break;
		case SNWorldDirNW:
			near_shadow_u |= NearbySpace.IsBlocked(x, y - 1, z + 1, SNWNearbyObjectTypeGround);
			near_shadow_l |= NearbySpace.IsBlocked(x - 1, y, z + 1, SNWNearbyObjectTypeGround);
			*rt = near_shadow_r && near_shadow_u;
			*lb = near_shadow_l && near_shadow_b;
			break;
		}
	}

	return ret;
}

Void SNWorld::UpdateWorldTime()
{
	WorldTime += 10;

	if ((SNWTimeHour * SNSystemConfig::FPS / 1000) <= WorldTime)
	{
		TimeHour = (Int32)SNMath::Increment(TimeHour, 0, SNWTimeStepNum - 1);
		GlobalLight = TimeToGlobalLight[TimeHour];
		WorldTime = 0;
	}

	return;
}
