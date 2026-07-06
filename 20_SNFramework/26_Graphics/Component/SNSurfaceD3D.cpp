#include "SNSurfaceD3D.h"
#include "SNSystemConfig.h"
#include "SND3D.h"

// サーフェスクラス

	// コンストラクタ
SNSurfaceD3D::SNSurfaceD3D()
{
	return;
}

// デストラクタ
SNSurfaceD3D::~SNSurfaceD3D()
{
	DeleteDeviceContext();

	return;
}

// サーフェス生成
Void SNSurfaceD3D::CreateSurface(Int32 width, Int32 height)
{
	// 基底クラス側での処理をしたくないので無処理

	return;
}

// サーフェス破棄
Void SNSurfaceD3D::DeleteSurface()
{
	// 基底クラス側での処理をしたくないので無処理

	return;
}

// 幅取得
Int32 SNSurfaceD3D::GetWidth()
{
	// D3Dのサイズは画面サイズと一致
	return SNSystemConfig::ScreenWidth; 
}

// 高さ取得
Int32 SNSurfaceD3D::GetHeight()
{
	// D3Dのサイズは画面サイズと一致
	return SNSystemConfig::ScreenHeight;
}


// DC生成
Void SNSurfaceD3D::CreateDeviceContext()
{
	DeleteDeviceContext();
	DeviceContext = SND3D::GetDC();
	return;
}

// DC破棄
Void SNSurfaceD3D::DeleteDeviceContext()
{
	if (DeviceContext != nullptr)
	{
		SND3D::ReleaseDC();
		DeviceContext = nullptr;
	}
	return;
}

// ビットマップ選択
Void SNSurfaceD3D::SetBitmap(Handle bitmap)
{
	// 基底クラス側での処理をしたくないので無処理

	return;
}

// ビットマップ選択解除
Void SNSurfaceD3D::ReleaseBitmap()
{
	// 基底クラス側での処理をしたくないので無処理

	return;
}
