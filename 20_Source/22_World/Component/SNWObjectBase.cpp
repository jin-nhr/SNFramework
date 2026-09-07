#include "SNWObjectBase.h"
#include "SNMath.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"
#include "SNMapchip.h"
#include "SNWObjectchip.h"

SNWObjectBase::SNWObjectBase()
{
	InitObjectInfo();
	EventList.Allocate(EventMaxNum);

	return;
}

SNWObjectBase::~SNWObjectBase()
{
	EventList.Free();
	return;
}

Void SNWObjectBase::Initialize()
{
	InitObjectInfo();
	EventList.Clear();

	return;
}

Void SNWObjectBase::Terminate()
{
	EventList.Free();
	return;
}

// ObjectInfo参照
SNWObjectInfo* SNWObjectBase::RefInfo()
{
	return &Info;
}

Void SNWObjectBase::InitObjectInfo()
{
	SNWObjectInfo* info;

	info = &Info;

	info->Code = SNWObjectchip::WObjectCodeMaleOrg;
	info->AnimationStep = 0;
	info->Enable = false;
	info->Visible = false;
	info->Weight = false;
	info->State = SNWObjectStateIdle;
	info->PrevState = SNWObjectStateNum;	// いずれの状態とも一致しない値としておく
	info->FrontDir = SNWorldDirS;
	info->Pos.X = 0;
	info->Pos.Y = 0;
	info->Pos.Z = 0;
	info->Acceleration.X = 0;
	info->Acceleration.Y = 0;
	info->Acceleration.Z = 0;
	info->Speed.X = 0;
	info->Speed.Y = 0;
	info->Speed.Z = 0;

	return;
}

// コード取得
UInt16 SNWObjectBase::GetCode()
{
	return SNWObjectchip::Data[Info.Code].Code;
}

// アニメステート取得
SNWObjectchip::SNWActState SNWObjectBase::GetActState()
{
	SNWObjectchip::SNAnimationCode anm_code = SNWObjectchip::Data[Info.Code].AnimeCode[Info.State];
	SNWObjectchip::SNWActState act_code = SNWObjectchip::AnimationInfo[anm_code].StepInfo[Info.AnimationStep].State;

	return act_code;
}

// 方向計算
SNWorldDir SNWObjectBase::CalcDir(SNWorldDir base_dir)
{
	return (SNWorldDir)(((Info.FrontDir - SNWorldDirN + SNWorldDirNW) + (base_dir - SNWorldDirN)) % (SNWorldDirNW) + SNWorldDirN);

}

Void SNWObjectBase::Notify(SNWObjectEventParam* param)
{
	SNListContainer* it = EventList.InsertLast();
	SNWObjectEventParam* lst_data;

	if (it != nullptr)
	{
		lst_data = (SNWObjectEventParam*)&(it->UserData);
		*lst_data = *param;
	}

	return;
}


Void SNWObjectBase::Update()
{
	SNListContainer* it;
	SNWObjectEventParam evt;

	// リストすべてを参照
	while (EventList.GetNum() != 0)
	{
		// 先頭を取る
		it = EventList.GetTop();

		evt = *((SNWObjectEventParam*)&(it->UserData));

		// イベント処理
		EventFunc(&evt);

		// 処理済みを削除
		EventList.Remove(it);
	}

	// オブジェクトのアニメ更新
	UpdateAnimation();

	// 速度更新
	UpdateSpeed();

	return;
}

Void SNWObjectBase::EventFunc(SNWObjectEventParam* param)
{
	switch (param->Event)
	{
	case SNWObjectEventStop:
		EventFuncStop(param);
		break;
	case SNWObjectEventWalk:
		EventFuncWalk(param);
		break;
	case SNWObjectEventJog:
		EventFuncJog(param);
		break;
	case SNWObjectEventJump:
		EventFuncJump(param);
		break;
	}
}

// パラメータなし
Void SNWObjectBase::EventFuncStop(SNWObjectEventParam* param)
{
	// 状態を設定
	Info.PrevState = Info.State;
	Info.State = SNWObjectStateIdle;

	// 目標速度に0をセット
	Info.Acceleration.X = 0.0f;
	Info.Acceleration.Y = 0.0f;

	return;
}

// Param1 - Dir
Void SNWObjectBase::EventFuncWalk(SNWObjectEventParam* param)
{
	SNWorldDir dir = (SNWorldDir)param->Param1;
	Int8 angle = (Int8)param->Param2;

	// 状態を設定
	Info.PrevState = Info.State;
	Info.State = SNWObjectStateWalk;

	// 向きを設定
	Info.FrontDir = dir;

	// 目標速度をセット
	Info.Acceleration.X = (SNWObjectchip::Data[Info.Code].SpeedWalk * SNMath::GetCos(angle) / SNTrigFuncMax * SNSystemConfig::FPS) / 1000.0f;
	Info.Acceleration.Y = -1 * (SNWObjectchip::Data[Info.Code].SpeedWalk * SNMath::GetSin(angle) / SNTrigFuncMax * SNSystemConfig::FPS) / 1000.0f; // 画面と三角関数はY軸で反転している

	return;
}


Void SNWObjectBase::EventFuncJog(SNWObjectEventParam* param)
{
	SNWorldDir dir = (SNWorldDir)param->Param1;
	Int8 angle = (Int8)param->Param2;

	// 状態を設定
	Info.PrevState = Info.State;
	Info.State = SNWObjectStateJog;

	// 向きを設定
	Info.FrontDir = dir;

	// 目標速度をセット
	Info.Acceleration.X = (SNWObjectchip::Data[Info.Code].SpeedJog * SNMath::GetCos(angle) * SNSystemConfig::FPS / SNTrigFuncMax) / 1000.0f;
	Info.Acceleration.Y = -1 * (SNWObjectchip::Data[Info.Code].SpeedJog * SNMath::GetSin(angle) * SNSystemConfig::FPS / SNTrigFuncMax) / 1000.0f; // 画面と三角関数はY軸で反転している

	return;
}


Void SNWObjectBase::EventFuncJump(SNWObjectEventParam* param)
{
	SNWorldDir dir = (SNWorldDir)param->Param1;
	Int8 angle = (Int8)param->Param2;

	// 状態を設定
	Info.PrevState = Info.State;
	Info.State = SNWObjectStateJump;

	// 目標速度をセット
	Info.Acceleration.Z = (SNWObjectchip::Data[Info.Code].SpeedJump * SNSystemConfig::FPS) / 1000.0f;

	return;
}

Void SNWObjectBase::UpdateState()
{
	// フィードバック情報の確認


	// 落下速度ありならFall


	// Fallから落下速度なしになったらIdle



	return;
}


Void SNWObjectBase::UpdateAnimation()
{
	SNWObjectchip::SNAnimationCode anime_code = SNWObjectchip::Data[Info.Code].AnimeCode[Info.State];

	if (Info.State != Info.PrevState)
	{
		Info.AnimationStep = 0;

		// タイマ起動
		Timer.Start((UInt16)SNWObjectchip::AnimationInfo[anime_code].StepInfo[Info.AnimationStep].Wait);
	}

	else
	{
		// ウェイト時間経過
		if (Timer.IsTimeout())
		{
			// Stepを進める
			if (SNWObjectchip::AnimationInfo[anime_code].Loop)
			{
				// ループあり
				Info.AnimationStep = (UInt32)SNMath::Increment(Info.AnimationStep, 0, SNWObjectchip::AnimationInfo[anime_code].StepNum - 1);
			}
			else
			{
				// ループなし
				Info.AnimationStep = (UInt32)SNMath::Saturate(Info.AnimationStep + 1, 0, SNWObjectchip::AnimationInfo[anime_code].StepNum - 1);
			}

			// タイマ起動
			Timer.Start((UInt16)SNWObjectchip::AnimationInfo[anime_code].StepInfo[Info.AnimationStep].Wait);
		}
	}

	return;
}

Void SNWObjectBase::UpdateSpeed()
{
	// 目標速度から現在速度を計算する
	Info.Speed.X += (Info.Acceleration.X - Info.Speed.X);
	Info.Speed.Y += (Info.Acceleration.Y - Info.Speed.Y);

	return;
}

