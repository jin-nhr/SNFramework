#include "SNWorld.h"
#include "SNSystemConfig.h"


SNWorldPos SNWorld::CurrentPos = {0};

SNWGround SNWorld::Ground;

Boolean SNWorld::Run = false;

// 初期化
Void SNWorld::Initialize()
{
	CurrentPos = {0};
	Run = false;

	Ground.Initialize();

	return;
}

// 終了
Void SNWorld::Terminate()
{
	Ground.Terminate();

	return;
}

Void SNWorld::Start()
{
	Run = true;
	return;
}

Void SNWorld::End()
{
	Run = false;
	return;
}

// 更新
Void SNWorld::Update()
{
	if (Run)
	{
		Ground.Update(&CurrentPos);
	}

	return;
}

// 描画
Void SNWorld::Draw(SNBitmap* target, SNWorldPos* target_pos, SNWorldDir dir)
{
	SNWorldShadowDir shadow_dir;

	if (Run)
	{
		shadow_dir = CalcShadowDir(dir);

		Ground.Draw(target, target_pos, true, dir, shadow_dir, SNSystemConfig::WorldDispRange, SNSystemConfig::WorldDispRangeZ);
	}

	return;
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
Void SNWorld::WriteGroundData(SNMapchip::SNMapchipCode code)
{
	Ground.Write(code);

	return;
}

// 影方向計算
SNWorldShadowDir SNWorld::CalcShadowDir(SNWorldDir dir)
{
	SNWorldDir light = SNWorldDirNW;
	SNWorldShadowDir ret = SNWorldShadowNon;

	switch (dir)
	{
	case SNWorldDirCenter:
		ret = SNWorldShadowNon;
		break;

	case SNWorldDirN:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
		case SNWorldDirNE:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirE:
		case SNWorldDirSE:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirS:
		case SNWorldDirSW:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirW:
		case SNWorldDirNW:
			ret = SNWorldShadowDirLT;
			break;
		}
		break;
	case SNWorldDirNE:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirNE:
		case SNWorldDirE:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirSE:
		case SNWorldDirS:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirSW:
		case SNWorldDirW:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirNW:
			ret = SNWorldShadowDirLT;
			break;
		}
		break;
	case SNWorldDirE:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
		case SNWorldDirNE:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirE:
		case SNWorldDirSE:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirS:
		case SNWorldDirSW:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirW:
		case SNWorldDirNW:
			ret = SNWorldShadowDirLB;
			break;
		}
		break;
	case SNWorldDirSE:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirNE:
		case SNWorldDirE:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirSE:
		case SNWorldDirS:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirSW:
		case SNWorldDirW:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirNW:
			ret = SNWorldShadowDirLB;
			break;
		}
		break;
	case SNWorldDirS:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
		case SNWorldDirNE:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirE:
		case SNWorldDirSE:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirS:
		case SNWorldDirSW:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirW:
		case SNWorldDirNW:
			ret = SNWorldShadowDirRB;
			break;
		}
		break;
	case SNWorldDirSW:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirNE:
		case SNWorldDirE:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirSE:
		case SNWorldDirS:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirSW:
		case SNWorldDirW:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirNW:
			ret = SNWorldShadowDirRB;
			break;
		}
		break;
	case SNWorldDirW:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
		case SNWorldDirNE:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirE:
		case SNWorldDirSE:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirS:
		case SNWorldDirSW:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirW:
		case SNWorldDirNW:
			ret = SNWorldShadowDirRT;
			break;
		}
		break;
	case SNWorldDirNW:
		switch (light)
		{
		case SNWorldDirCenter:
			ret = SNWorldShaodwDirT;
			break;
		case SNWorldDirN:
			ret = SNWorldShadowDirRT;
			break;
		case SNWorldDirNE:
		case SNWorldDirE:
			ret = SNWorldShadowDirRB;
			break;
		case SNWorldDirSE:
		case SNWorldDirS:
			ret = SNWorldShadowDirLB;
			break;
		case SNWorldDirSW:
		case SNWorldDirW:
			ret = SNWorldShadowDirLT;
			break;
		case SNWorldDirNW:
			ret = SNWorldShadowDirRT;
			break;
		}
		break;
	}

	return ret;
}
