#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"

// デバッグアプリ OFF
// 遷移コード: Code0 = Debug ON

class SNDebugAppOff : public virtual SNScene
{
public:
	// コンストラクタ
	SNDebugAppOff();

	// デストラクタ
	virtual ~SNDebugAppOff();

protected:

	virtual Void OnCycle();

private:

};

