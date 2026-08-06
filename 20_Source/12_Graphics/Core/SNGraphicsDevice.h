#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsContext.h"
#include "SNBitmap.h"

class SNGraphicsDevice
{
public:

	////////////////////////////////////////////
	// 初期化
	static Void Initialize();

	// デバイス生成
	static Void CreateDevice();

	// SwapChain生成
	static Void CreateSwapChain();

	// RTV生成
	static Void CreateRTV();

	static Void CreateD2DFactory();
	static Void CreateDeviceContext();

	// サーフェス生成
	static Void CreateSurface();

	// SRV生成
	static Void CreateSRV();

	// クアッド頂点バッファ生成
	static Void CreateFullscreenQuad();

	// シェーダー生成
	static Void CreateShaders();

	// サンプラー生成
	static Void CreateSampler();

	// WIC生成
	static Void CreateWIC();

	////////////////////////////////////////////
	// 終了
	static Void Terminate();

	// デバイス解放
	static Void ReleaseDevice();
	
	// SwapChain解放
	static Void ReleaseSwapChain();

	// RTV解放
	static Void ReleaseRTV();
	
	static Void ReleaseFactory();
	static Void ReleaseDeviceContext();

	// サーフェス解放
	static Void ReleaseSurface();
	// 
	// SRV解放
	static Void ReleaseSRV();

	// クアッド頂点バッファ解放
	static Void ReleaseFullscreenQuad();

	// シェーダー解放
	static Void ReleaseShaders();

	// サンプラー解放
	static Void ReleaseSampler();

	// WIC解放
	static Void ReleaseWIC();

	////////////////////////////////////////////
	// その他

	static Void Restore(SNSize* size);

	// ステートクリア
	static Void ClearState();

	// フリップ処理
	static Void Flip(SNRect* rect);

	// フルスクリーン設定
	static Void SetFullScreen(Boolean fs);

	// リサイズ
	static Void ResizeBuffer(SNSize* size);

	// 画面サイズ取得
	static Void GetWindowSize(SNSize* size);

public:

	static Handle Device;
	static Handle DeviceContext;
	static Handle SwapChain;
	static Handle RenderTargetView;
	static Handle ShaderResourceView;

	static Handle VertexBuffer;
	static Handle InputLayout;
	static Handle VertexShader;
	static Handle PixelShader;
	static Handle SamplerState;

	static Handle D2DFactory;
	static Handle D2DDevice;
	static SNGraphicsContext D2DGraphicsContext;
	static SNBitmap D2DTargetBitmap;
};
