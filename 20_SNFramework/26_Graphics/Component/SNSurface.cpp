#include "SNSurface.h"

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

