#include "SNSurface.h"
#include "../System/SNWindowsAPI.h"

// サーフェスクラス

// コンストラクタ
SNSurface::SNSurface()
{
	return;
}

// デストラクタ
SNSurface::~SNSurface()
{
	// デバイスコンテキスト削除
	DeleteDeviceContext();

	return;
}

