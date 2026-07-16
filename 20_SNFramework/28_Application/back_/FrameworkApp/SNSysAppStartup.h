#pragma once
#include "SNFrameworkInternal.h"
#include "SNLayerController.h"
#include "SNGraphics.h"
#include "SNGUISystemText.h"

// システムアプリ 起動準備
class SNSysAppStartup : public virtual SNLayerController
{
public:
	// コンストラクタ
	SNSysAppStartup();

	// デストラクタ
	virtual ~SNSysAppStartup();

protected:
	// 初期化
	virtual Void OnInitialize();


private:
	SNGUISystemText	LblCenter;
};

