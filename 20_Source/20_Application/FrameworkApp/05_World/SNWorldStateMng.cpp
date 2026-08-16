#include "SNWorldStateMng.h"

Void SNWorldStateMng::OnInitialize()
{
	SetSceneNum(2);

	//                0   1   2   3
	SetScene(&Off,    1, -1, -1, -1);	// 0 - OFF
	SetScene(&World, -1, -1, -1, -1);	// 1 - World

	return;
}

Void SNWorldStateMng::OnTerminate()
{
	return;
}

