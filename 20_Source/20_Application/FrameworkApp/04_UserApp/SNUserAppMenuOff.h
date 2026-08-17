#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"



// メニューOFF
// 0 - メニュー表示
// 1 -
// 2 -
// 3 - 


class SNUserAppMenuOff : public SNScene
{
public:
	SNUserAppMenuOff();
	virtual ~SNUserAppMenuOff();

protected:
	virtual Boolean OnGamePad1();

};

